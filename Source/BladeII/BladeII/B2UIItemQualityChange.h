// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
//#include "CoreMinimal.h"
//#include "B2UIBackWidget.h"
//#include "B2Inventory.h"
//#include "B2LobbyInventory.h"
//#include "B2LobbyUserWidget.h"
//#include "B2DynItemIconRow_LobbyInven.h"
//#include "B2UIBackWidgetBase.h"
//#include "B2UIItemQualityChange.generated.h"
//
///**
// * 
// */
//UCLASS()
//class BLADEII_API UB2UIItemQualityChange : public UB2LobbyUserWidget, public IB2UIBackWidget
//{
//	GENERATED_BODY()
//
//	static class AB2LobbyGameMode* CachedLobbyGM;
//
//private:
//	UFUNCTION(BlueprintCallable, Category = "BladeII")
//		void VisibleCloseButton();
//	UFUNCTION(BlueprintCallable, Category = "BladeII")
//		void InvisibleCloseButton();
//
//	void DisableChangeButtons();
//	
//protected:
//	virtual void CloseWidgetDelegate() override;
//	virtual void CacheAssets() override;
//
//	void UpdateStaticText();
//	void UpdateDynamicText();
//	
//
//	UFUNCTION(BlueprintCallable, Category = "BladeII")
//		void OnItemQualityApplyAnimEnd();
//
//	void RefreshOptionChangeCostColor();
//	bool HasRemainUnAppliedQuality();
//	bool IsEnableButtonQualityChange();
//	bool IsEnableButtonOptionCashChange();
//	void CreateHammerPopup(UObject* WorldContextObject, FB2Item& InItemData);
//
//	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
//		void OnItemQualityApplied_BP();
//	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
//		void OnGenerateNewItemQuality_BP();
//	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
//		void OnGenerateOtherItemQuality_BP();
//
//	void ResponseGetItemRandomQuality(FB2ResponseGetItemRandomQualityPtr QualityItemDataPtr);
//	void ResponseGenerateItemRandomQuality(FB2ResponseGenerateItemRandomQualityPtr GenerateItemRandomQualityPtr);
//	void ResponseApplyItemRandomQuality(FB2ResponseApplyItemRandomQualityPtr ApplyItemRandomQualityPtr);
//	void ResponseDiscardItemRandomQuality(FB2ResponseDiscardItemRandomQualityPtr DiscardItemRandomQualityPtr);
//
//	UFUNCTION()
//		void OnClickBTN_Close();
//	UFUNCTION()
//		void OnClickBTN_Confirm();
//	UFUNCTION()
//		void OnClickBTN_ApplyQuality();
//	UFUNCTION()
//		void OnClickBTN_ChangeQuality();
//	UFUNCTION()
//		void OnClickBTN_ChangeHammer();
//
//	void HammerCycle();
//	
//public:
//	UB2UIItemQualityChange(const FObjectInitializer& ObjectInitializer);
//
//	void SubscribeEvents();
//	void UnsubscribeEvents();
//
//	void SetTargetItem(FB2Item& TargetItem, AB2LobbyGameMode* TransientLobbyGM);
//	void SwitchTargetItem(FB2Item& TargetItem);
//
//	void SetTargetItemIcon(const FB2Item& TargetItem);
//	void SetAfterItemIcon(const FB2Item& OriginalItem, int32 qualityValue);
//	void SetOriginalQualityDetail(const FB2Item& TargetItem);
//	void RequestItemQualityApply();
//
//	void SetApplyQualityDetail(const FB2Item& TargetItem);
//
//	void RequestNewItemQuality(int64 HammerID);
//
//	void CreateLobbyHammerChangePopup(UObject* WorldContextObject, FB2Item& InItemData);
//	void CloseHammerChangePopup();
//
//
//protected:
//	UPROPERTY(Transient)
//		class UB2LobbyInventory* CachedInventory;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
//		FLinearColor EnoughColor;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
//		FLinearColor NotEnoughColor;
//	
//
//private:
//	TArray<int64> AllIngredIDs;
//	TArray<int32> QualityValues;
//	int64 SelectedHammer;
//
//	bool	bEnableApplyQuality;
//	bool	bItemQualityApplied;
//	bool	ment2Close = false;
//
//	int64	ItemUniqueInstanceID;
//	int32	ItemReferenceID;
//
//	int32	OriginalOptionCount;
//	EPCClass ItemOwnerPCClass;
//
//	int32 hammercycleCount;
//
//	FB2Item CachedApplyItemQualityForAnimation;
//	FB2Item CachedItem;
//	int32 CachedQuality=0;
//
//private:
//	TArray<EItemClass> OriginOption;
//	TArray<EItemClass> ChangedOption;
//	TArray<EItemClass> AppliedOption;
//
//	TWeakObjectPtr<UB2RichTextBlock> RTB_OptionChangeTitle;
//	TWeakObjectPtr<class UB2UIQualityChangeDetail> UIP_OriginalOption;
//	TWeakObjectPtr<class UB2UIQualityChangeDetail> UIP_ChangedOption;
//	TWeakObjectPtr<UTextBlock> TB_OriginalOption;
//	TWeakObjectPtr<UTextBlock> TB_ChangedOption;
//	TWeakObjectPtr<class UB2DynItemIcon_LobbyInven> UIP_BeforeItemIcon;
//	TWeakObjectPtr<class UB2DynItemIcon_LobbyInven> UIP_AfterItemIcon;
//	TWeakObjectPtr<UTextBlock> TB_ExpectedQuality;
//	TWeakObjectPtr<UTextBlock> TB_MinQuality;
//	TWeakObjectPtr<UTextBlock> TB_MiddleQuality;
//	TWeakObjectPtr<UTextBlock> TB_MaxQuality;
//	TWeakObjectPtr<UB2RichTextBlock> RTB_ChangeDesc;
//	TWeakObjectPtr<UB2Button> BTN_ApplyQuality;
//	TWeakObjectPtr<UTextBlock> TB_ApplyQuality;
//	TWeakObjectPtr<UB2ButtonGoodsShortcutToolTip> BTN_ChangeHammer;
//	TWeakObjectPtr<UTextBlock> TB_ChangeHammer;
//	TWeakObjectPtr<UB2ButtonGoodsShortcutToolTip> BTN_ChangeQuality;
//	TWeakObjectPtr<UTextBlock> TB_ChangeQuality;
//	TWeakObjectPtr<UTextBlock>	TB_ChangeQualityCost;
//	TWeakObjectPtr<UB2Button> BTN_Close;
//	TWeakObjectPtr<UB2Button> BTN_OptionInfo;
//
//	TWeakObjectPtr<UImage> IMG_ChangeQuality;
//	TWeakObjectPtr<UTextBlock> TB_QualityChangeCost;
//
//	TWeakObjectPtr<UPanelWidget> X_HammerChangePopup;
//
//
//	TWeakObjectPtr<UB2Button> BTN_Confirm;
//	TWeakObjectPtr<UTextBlock> TB_Confirm;
//	TWeakObjectPtr<UTextBlock> TB_ChangeResultTitle;
//	TWeakObjectPtr<class UB2UIQualityChangeDetail> UIP_ResultOptionDetail;
//
//	TArray<std::shared_ptr<class issue>> Issues;
//};
//
//UCLASS()
//class BLADEII_API UB2UIQualityChangeDetail : public UB2UIWidgetBase
//{
//	GENERATED_BODY()
//
//private:
//	FSlateColor GetCompareTextColor(const float CompareValue, const bool FindOption);
//	FText	    MakeParenthesesText(const float CompareValue, const FText& CompareText, const bool FindOption);
//
//protected:
//	virtual void CacheAssets() override;
//
//public:
//	virtual void Init() override;
//	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
//
//	void ClearQualityTextList();
//	void SetOptionText(EItemClass ClassType, float value);
//	void SetCompareQualityText(EItemClass ClassType, float beforeValue, float afterValue);
//	//void SetCompareOptionText(int32 OptionIndex, EItemOption OptionType, EPCClass OwnerClassType, float RawOptionAmount, const TArray<FItemOption>& OriginOption);
//	//void CreateLobbyItemAfterQualityPopup(UObject* WorldContextObject, FB2Item& InItemData);
//
//	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
//		void OnItemQualityGenerated_BP(int32 OptionCount);
//
//private:
//	TArray<TWeakObjectPtr<class UB2DynText_Multiple>> OptionTextList;
//	TWeakObjectPtr<UImage> IMG_Empty;
//
//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
//		FSlateColor NewTextColor;
//	
//};