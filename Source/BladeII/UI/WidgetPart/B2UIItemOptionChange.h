// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "../Widget/B2ButtonGoodsShortcutToolTip.h"
#include "../B2RichTextBlock.h"
#include "B2UIItemOptionChange.generated.h"


UCLASS()
class BLADEII_API UB2UIItemOptionChange : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

private:
	void SortOptionArray(TArray<FItemOption>& OptionArray);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void VisibleCloseButton();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
		void InvisibleCloseButton();

protected:
	virtual void OnCloseComplete() override;

	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

	virtual void CloseWidgetDelegate() override;

	bool HasRemainUnAppliedOption();
	bool IsEnableButtonOptionChange();
	bool IsEnableButtonOptionCashChange();

	void SubscribeEvents();
	void UnsubscribeEvents();

	void ResponseGenerateItemRandomOption(FB2ResponseGenerateItemRandomOptionPtr GenerateItemRandomOptionPtr);
	void ResponseApplyItemRandomOption(FB2ResponseApplyItemRandomOptionPtr ApplyItemRandomOptionPtr);
	void ResponseDiscardItemRandomOptionn(FB2ResponseDiscardItemRandomOptionPtr DiscardItemRandomOptionPtr);

	void RefreshOptionChangeCostColor();

	UFUNCTION()
		void OnClickBTN_ApplyOption();
	UFUNCTION()
		void OnClickBTN_ChangeOption();
	UFUNCTION()
		void OnClickBTN_ChangeOptionCash();
	UFUNCTION()
		void OnClickBTN_Close();
	UFUNCTION()
		void OnClickBTN_OptionInfo();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnItemOptionApplied_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnGenerateNewItemOption_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnGenerateOtherItemOption_BP();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnItemOptionApplyAnimEnd();

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetTargetItem(const FB2Item& TargetItem);

	void SetTargetItemIcon(const FB2Item& TargetItem);
	void SetOriginalOptionDetail(const FB2Item& TargetItem);
	void SetNewOptionDetail(const TArray<FItemOption>& NewItemOptions);
	void SetApplyOptionDetail(const FB2Item& TargetItem);
	void ClearOptionDetail();
	
	void RequestItemOptionApply(/*...*/);

	void RequestNewItemOption(int32 InButType);

	void CreateLobbyItemAfterOptionPopup(UObject* WorldContextObject, FB2Item& InItemData);
	
	UFUNCTION()
	void CloseAfterOptionPopup();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	FLinearColor EnoughColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	FLinearColor NotEnoughColor;

private:

	int64	ItemUniqueInstanceID;
	int32	ItemReferenceID;

	EPCClass ItemOwnerPCClass;

	bool	bEnableApplyOption;

	bool	bItemOptionApplied;

	int32	NewOptionCount;
	int32	OriginalOptionCount;

	FB2Item CachedApplyItemOptionForAnimation;
	FB2Item CachedItem;

private:
	TArray<FItemOption> OriginOption;

	TWeakObjectPtr<UB2RichTextBlock> RTB_OptionChangeTitle;
	TWeakObjectPtr<class UB2UIOptionChangeDetail> UIP_OriginalOption;
	TWeakObjectPtr<class UB2UIOptionChangeDetail> UIP_ChangedOption;
	TWeakObjectPtr<UTextBlock> TB_OriginalOption;
	TWeakObjectPtr<UTextBlock> TB_ChangedOption;
	TWeakObjectPtr<class UB2DynItemIcon_LobbyInven> UIP_ItemIcon;
	TWeakObjectPtr<UB2RichTextBlock> RTB_ChangeDesc;
	TWeakObjectPtr<UB2Button> BTN_ApplyOption;
	TWeakObjectPtr<UTextBlock> TB_ApplyOption;
	TWeakObjectPtr<UB2ButtonGoodsShortcutToolTip> BTN_ChangeOption;
	TWeakObjectPtr<UTextBlock> TB_OptionChange;
	TWeakObjectPtr<UTextBlock>	TB_OptionChangeCost;
	TWeakObjectPtr<UB2ButtonGoodsShortcutToolTip> BTN_ChangeOptionCash;
	TWeakObjectPtr<UTextBlock> TB_OptionChangeCash;
	TWeakObjectPtr<UTextBlock>	TB_OptionChangeCostCash;
	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_OptionInfo;

	TWeakObjectPtr<UPanelWidget> X_MainPopup;
	TWeakObjectPtr<UPanelWidget> X_AfterOptionPopup;


	TWeakObjectPtr<UB2Button> BTN_Confirm;
	TWeakObjectPtr<UTextBlock> TB_Confirm;
	TWeakObjectPtr<UTextBlock> TB_ChangeResultTitle;
	TWeakObjectPtr<class UB2UIOptionChangeDetail> UIP_ResultOptionDetail;

	TArray<std::shared_ptr<class issue>> Issues;
};


UCLASS()
class BLADEII_API UB2UIOptionChangeDetail : public UB2UIWidgetBase
{
	GENERATED_BODY()

private:
	FSlateColor GetCompareTextColor(const float CompareValue, const bool FindOption);
	FText	    MakeParenthesesText(const float CompareValue, const FText& CompareText, const bool FindOption);

protected:
	virtual void CacheAssets() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void ClearOptionTextList();
	void SetOptionText(int32 OptionIndex, EItemOption InOptionType, EPCClass OwnerClass, float RawOptionAmount);
	void SetCompareOptionText(int32 OptionIndex, EItemOption InOptionType, EPCClass OwnerClass, float RawOptionAmount, const TMap<EItemOption, float>& CompareOption);
	void SetCompareOptionText(int32 OptionIndex, EItemOption OptionType, EPCClass OwnerClassType, float RawOptionAmount, const TArray<FItemOption>& OriginOption);
	void CreateLobbyItemAfterOptionPopup(UObject* WorldContextObject, FB2Item& InItemData);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnItemOptionGenerated_BP(int32 OptionCount);

private:
	TArray<TWeakObjectPtr<class UB2DynText_Multiple>> OptionTextList;
	TWeakObjectPtr<UImage> IMG_Empty;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		FSlateColor NewTextColor;
};
