// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIStore.h"
#include "B2UIPackageStore.h"
#include "B2UICommonTab.h"
#include "B2UIPackageTypePanel.generated.h"

enum class EREWARD_STATE : uint8
{
	NotBuy = 0,
	Impossible,
	Possible,
	Rewarded,
	Fail,		// Only Used ReachLevel
};

enum ELevelPackageType : uint8
{
	LevelPackage_Levelup = 0,
	LevelPackage_ReachLevel = 1,
};


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPackageTypePanel : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	UB2UIPackageTypePanel(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void InitPanelInfo(int32 nType);
	virtual void UpdatePanelInfo();

	virtual void DoMarkRedDotPanel();

public:
	ESTORE_SLOT_WIDGET_TYPE GetProductWidget(int32 PackageID);
	class UMaterialInstance* GetProductIcon(int32 PackageID, int32 OptionalItemId);
	ESTORE_SLOT_EFFECT_TYPE GetProductIconEffect(int32 ProductId);
	FText GetProductName(int32 PackageID, int32 OptionalItemId);
	void GetBonusIconAndName(EStoreItemType BonusType, UMaterialInstance*& OutBonusIcon, FText& OutBonusName, int32 OptionalItemTemplateId);
	ESTORE_BUYPOPUP_WIDGET_TYPE GetPopupWidgetType(int32 PackageID);
	EPACKAGE_TYPE GetPackageType(int32 PackageID);

	FORCEINLINE int32 GetCurrentStoreCategory() { return CurrentStoreCategory; }

protected:

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<class UB2UIStoreItemSlot>				DefaultItemSlot;

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<class UB2UIStoreProductIconData>		ProductIconData;

public:
	FButtonOneParamDelegate ButtonOneParamDelegate;

private:
	int32 CurrentStoreCategory;

};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPackageTypePanel_Basic : public UB2UIPackageTypePanel
{
	GENERATED_BODY()

protected:
	UB2UIPackageTypePanel_Basic(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void InitPanelInfo(int32 nType) override;
	virtual void UpdatePanelInfo() override;

private:

	void SetBannerInfo();

private:

	TWeakObjectPtr<UWidgetSwitcher>			WS_Banner;

	TWeakObjectPtr<UHorizontalBox>			HB_ItemList;
	TWeakObjectPtr<UB2ScrollBox>			SB_List;

private:

	TArray<TWeakObjectPtr<UB2UIWidgetBase>> ItemSlots;

};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPackageTypePanel_Flatrate : public UB2UIPackageTypePanel
{
	GENERATED_BODY()

protected:
	UB2UIPackageTypePanel_Flatrate(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;
	virtual	void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void InitPanelInfo(int32 nType) override;
	virtual void UpdatePanelInfo() override;
	virtual void DoMarkRedDotPanel() override;
private:

	void SetBonusIcon();
	void SetRewardButton();

	void OnReciveRewardFlatRatePackageBonus(FB2ResponseRewardFlatRatePackageBonusPtr Ptr);

protected:

	UFUNCTION()
		void OnClickBTN_GetReward();
	UFUNCTION()
		void OnClickBTN_Detail();
	UFUNCTION()
		void BuyFlatRateItem(int32 StoreID);

	UFUNCTION()
		bool RedDotCondition_FlatRate();

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<class UB2UIPacakgeFlatrateItem>	FlatRateItem;

private:

	// List
	TWeakObjectPtr<UHorizontalBox>					HB_FlatrateList;

	// Bonus
	TWeakObjectPtr<UTextBlock>						STB_FlatrateTitle;
	TWeakObjectPtr<UTextBlock>						STB_FlatrateDesc;

	TWeakObjectPtr<UHorizontalBox>					HB_FlatrateReward;
	TWeakObjectPtr<UCheckBox>						CB_Weekly;
	TWeakObjectPtr<UTextBlock>						STB_Weekly;
	TWeakObjectPtr<UTextBlock>						TB_Weekly;
	TWeakObjectPtr<UCheckBox>						CB_Monthly;
	TWeakObjectPtr<UTextBlock>						STB_Monthly;
	TWeakObjectPtr<UTextBlock>						TB_Monthly;
	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_FlatrateReward;

	TWeakObjectPtr<UButton>							BTN_GetReward;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_ReceiveBtn;
	TWeakObjectPtr<UTextBlock>						STB_Complete;

	TWeakObjectPtr<UB2RichTextBlock>				RTB_WarningSummary;
	TWeakObjectPtr<UB2RichTextBlock>				RTB_FlatrateSummary;
	TWeakObjectPtr<UButton>							BTN_Detail;
	TWeakObjectPtr<UTextBlock>						STB_Detail;

	TWeakObjectPtr<UB2RichTextBlock>				RTB_SummaryInfo;
	TWeakObjectPtr<UButton>							BTN_SummaryDetail;
	TWeakObjectPtr<UTextBlock>						STB_SummaryDetail;
	TWeakObjectPtr<UTextBlock>						STB_Summary;

	TWeakObjectPtr<UWidgetSwitcher>					WS_Info;
	TWeakObjectPtr<UCanvasPanel>					CP_MoreAdventage;
	TWeakObjectPtr<UCanvasPanel>					CP_NormalInfo;

private:

	uint32 DeliveryResponseFlatratePackageBonusTicket;

};

DECLARE_DELEGATE(FReddotCheckLevelUpMission)

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPackageTypePanel_Level : public UB2UIPackageTypePanel
{
	GENERATED_BODY()

protected:

	UB2UIPackageTypePanel_Level(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

	virtual void InitPanelInfo(int32 nType) override;
	virtual void UpdatePanelInfo() override;
	virtual void DoMarkRedDotPanel() override;

	void OnClickTabButtonDelegate(int32 ButtonIndex, ELevelPackageType PackageType);
	
	void SetPackageProductMark(int32 ProductId);

	UFUNCTION()
		void OnClickTopTab(EPCClass selectedClass);

	UFUNCTION()
		void OnClickBTN_LvupBuy();
	UFUNCTION()
		void OnClickBTN_ReachBuy();
	UFUNCTION()
		void OnClickBTN_MissionReward();

	UFUNCTION()
		void OnClickBTN_Detail();

private:

	void CreateLeftTab();
	void SetSelectedLeftTab(int32 StoreID);
	FText GetLeftTabName(int32 PackageID);

	void UpdateLevelupPanel();
	void UpdateLevelupPackageLIst(TArray<FB2ShopLevelUpPackagePtr>& LevelupList);
	void UpdateReachLevelPanel();

	void SetRestrictData(int32 RestrictCount);
	void SetReachRewardState();

	void OnReciveLevelupReward(FB2ResponseRewardLevelUpPackagePtr RewardPtr);
	void OnReciveReachLevelReward(FB2ResponseRewardReachLevelPackagePtr RewardPtr);

	UFUNCTION()
		bool RedDotCondition_Gladiator();
	UFUNCTION()
		bool RedDotCondition_Assassin();
	UFUNCTION()
		bool RedDotCondition_Wizard();
	UFUNCTION()
		bool RedDotCondition_Fighter();

protected:

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<class UB2UIPacakgeLevelupListPart>		LevelupSlotItem;

	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<class UB2UIPacakgeLevelupLeftTab>		LeftTabItem;

private:

	EPCClass SelectedPCClass;
	int32 CurrentStoreID;
	int32 CurrentPackageID;
	float ScrollOffset_LeftTab;
	float ScrollOffset_List;

	// All used
	TArray<TWeakObjectPtr<class UB2UICommonTab>>	TopTabs;

	TWeakObjectPtr<UB2RichTextBlock>				RTB_LevelupSummary;
	TWeakObjectPtr<UButton>							BTN_Detail;
	TWeakObjectPtr<UTextBlock>						STB_Detail;

	TWeakObjectPtr<UB2ScrollBox>					SB_LeftTab;
	TWeakObjectPtr<UVerticalBox>					VB_LeftTab;

	TWeakObjectPtr<UWidgetSwitcher>					WS_Content;

	// Levelup content
	TWeakObjectPtr<UB2ScrollBox>					SB_LevelUpList;
	TWeakObjectPtr<UVerticalBox>					VB_LevelUpList;

	TWeakObjectPtr<UWidgetSwitcher>					WS_ClassImage;

	TWeakObjectPtr<UTextBlock>						TB_Price;
	TWeakObjectPtr<UB2RichTextBlock>				STB_ReceiveDesc;
	TWeakObjectPtr<UB2RichTextBlock>				TB_LevelPackageDesc;

	TWeakObjectPtr<UButton>							BTN_LvBuy;
	TWeakObjectPtr<UB2UIStoreProductCost>			UIP_Cost;

	// Reach Level (Reach)
	TWeakObjectPtr<UTextBlock>						STB_Reach_Title;
	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_Reach_Reward;
	TWeakObjectPtr<UTextBlock>						STB_Reach_Restrict;
	TWeakObjectPtr<UTextBlock>						TB_Reach_RestrictCount;

	TWeakObjectPtr<UButton>							BTN_Reach_Buy;
	TWeakObjectPtr<UTextBlock>						STB_Reach_RestrictComplete;
	TWeakObjectPtr<UB2UIStoreProductCost>			UIP_Reach_Cost;

	// Reach Level (Mission)
	TWeakObjectPtr<UTextBlock>						STB_Mission_Title;
	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_Mission_Reward;
	TWeakObjectPtr<UTextBlock>						TB_Mission_PeriodCount;

	TWeakObjectPtr<UWidgetSwitcher>					WS_Mission_RewardState;
	TWeakObjectPtr<UTextBlock>						STB_Mission_NeedPackage;
	TWeakObjectPtr<UButton>							BTN_Mission_Reward;
	TWeakObjectPtr<UTextBlock>						STB_Mission_Reward;
	TWeakObjectPtr<UTextBlock>						STB_Mission_CompleteReward;
	TWeakObjectPtr<UTextBlock>						STB_Mission_FailReward;

	//
	TWeakObjectPtr<UTextBlock>						STB_Reach_LimitLevel;
	TWeakObjectPtr<UTextBlock>						TB_Reach_LimitLevel;
	TWeakObjectPtr<UTextBlock>						TB_Reach_Warning;

	TWeakObjectPtr<UTextBlock>						TB_SalePeriod;
	TWeakObjectPtr<class UB2UIStoreEventLabel>		UIP_Store_Event_Label;

	// Ticket
	uint32 DeliveryResponseRewardLevelUpPackageTicket;
	uint32 DeliveryResponseRewardReachLevelPackageTicket;
};

///////////////////////////////////////////////////////////////////
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPacakgeLevelupListPart : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIPacakgeLevelupListPart(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

	void InitEventLevelupPart(FB2ShopLevelUpPackagePtr LevelItem);
	void UpdateVisibleState();

protected:

	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

private:
	UFUNCTION()
		void OnClickBTN_GetReward();

private:

	TWeakObjectPtr<class UB2UIRewardIcon>	UIP_RewardIcon;

	TWeakObjectPtr<UB2Button>				BTN_GetReward;

	TWeakObjectPtr<UTextBlock>				TB_GetReward;
	TWeakObjectPtr<UTextBlock>				TB_GetRewardPossible;
	TWeakObjectPtr<UTextBlock>				TB_GetRewardImPossible;
	TWeakObjectPtr<UTextBlock>				TB_CompleteReward;
	TWeakObjectPtr<UTextBlock>				TB_NeedPackage;

	TWeakObjectPtr<UOverlay>				O_GetRewardPossible;
	TWeakObjectPtr<UOverlay>				O_GetRewardImPossible;

	TWeakObjectPtr<UWidgetSwitcher>			WS_ButtonState;

protected:

	int32									ShopIndex;
	int32									PackageIndex;
	int32									PCLevel;
	EREWARD_STATE							RewardState;
	EPCClass								PCClass;
};

///////////////////////////////////////////////////////////////////

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPacakgeLevelupLeftTab : public UB2UICommonTab
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

protected:
	virtual void CacheAssets() override;

public:
	void BindReddot();
	void SetCurrentPackageID(int32 PackageID, bool ReachLevel = false);
	bool RedDotCondition_LeftTab();
	void SetLimitedTime(bool bIsLimited);

private:
	TWeakObjectPtr<UOverlay>				O_Limited;
	TWeakObjectPtr<UTextBlock>				TB_Limited;
	
	int32 CurrentPackageID;
	bool IsReachLevel;
};

DECLARE_DELEGATE_OneParam(FBuyFlatRateItem, int32)
///////////////////////////////////////////////////////////////////
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIPacakgeFlatrateItem : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIPacakgeFlatrateItem(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetFlatrateSlotWidget(class UMaterialInstance* ProductIcon, int32 SlotIconEffectType);
	void SetFlatrateItemData(FStoreProductData StoreData, FPackageProductData PacakgeData, EPACKAGE_TYPE PackageType);
	void SetPeriodData(int32 PackageID);
	void SetPackageProductMark(int32 ProductId);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

private:
	UFUNCTION()
		void OnClickBTN_BuyItem();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
		TSoftClassPtr<class UB2UIStoreItemSlotIcon>	StoreSlotWidget;

private:
	TWeakObjectPtr<UTextBlock>						TB_FlatrateItemTitle;

	TWeakObjectPtr<UOverlay>						OV_SlotWidget;

	TWeakObjectPtr<class UB2UIRewardIcon>			UIP_FlatrateItemReward;
	TWeakObjectPtr<UTextBlock>						TB_FlatrateItemDay;
	TWeakObjectPtr<UTextBlock>						TB_FlatrateItemAll;
	TWeakObjectPtr<UTextBlock>						TB_FlatrateItemDate;

	TWeakObjectPtr<UButton>							BTN_BuyItem;
	TWeakObjectPtr<UTextBlock>						TB_RestrictComplete;
	TWeakObjectPtr<UB2UIStoreProductCost>			UIP_Cost;

	TWeakObjectPtr<class UB2UIStoreEventLabel>		UIP_Store_Event_Label;

public:
	FBuyFlatRateItem								BuyDelegate;

private:
	int32											StoreProductId;
};
