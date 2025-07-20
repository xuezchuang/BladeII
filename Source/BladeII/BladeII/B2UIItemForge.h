// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BladeII.h"
#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2UIItemForgeTabButton.h"
#include "B2UIItemForgeListButton.h"
#include "B2UIItemForgeGroupTitle.h"
#include "B2UIItemForgeIngredientList.h"
#include "B2DynItemIcon_ItemForgeResult.h"
#include "B2UIFilterItemForge.h"
#include "../DataStore/B2ClientDataStore.h"
#include "B2UIItemForge.generated.h"

/**
 *
 */
#define  AVAILABLE_FORGE_QUANTITY 100
#define  INGREDIENTTITLE_MAINMATERIAL 1
#define  INGREDIENTTITLE_JUSTMATERIAL 2

UCLASS()
class BLADEII_API UB2UIItemForge : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	void ForgeInit();

	void UpdatePanelWidget();
	void ResponseItemForgeList(FB2ResponseGetItemForgeListPtr ItemForgeListPtr);
	void ResponseTryItemForge(FB2ResponseTryItemForgePtr TryItemForgePtr);

	void SetBasicData();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	float ConvertItemCountToSliderValue(int32 InItemCount);
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 ConvertSliderValueToItemCount(float InSliderValue);
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnChangeItemCountSliderBar(float SliderValue);
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void SetItemCountAndUpdateWidget(int32 InItemCount);
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 GetPreQuantityItemCount();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 GetNextQuantityItemCount();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 GetForgeItemCount();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	int32 GetForgeItemMaxCount();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void UpdateStaticText() override;

	void SetButtonClick(EPCClass InEPCClass);
	void SetvisibilityOfDetail(bool showing);
	void OnChangeLevelSlider(float InValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnExecute_BP();
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnExecuteAnimEnd();

private:
	void SubscribeEvent();
	void UnSubscribeEvent();
	bool bSubscribed;
	TArray<std::shared_ptr<issue>> Issues;

	void SetTabList();
	void SetAvailableItemList();
	void SetIngredientList();
	void SetRequiredGoods();
	void SetLimitation();
	void CheckIsEverythingOkay();

	void OnManualForgeListSet();

	void ResetForgeSetting();
	void ResetForgeSettingForFilter();
	void HideQuantityAdjust(bool hideIt);
	void AdjustQuantity(bool uiOnly = false);

	float GetDivideElement();
	void SetElementSize(int32 InMaxCount);

	void ResultEnd();
	void OutofTimeForLimited();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void EventChangedHeroMgmtPCClass(EPCClass InSelectedClass);

	UFUNCTION()
	void OnClickedBTNForgeMenu();
	UFUNCTION()
	void OnClickedBTNUpgradeMenu();

	UFUNCTION()
	void OnClickedBTNGladiator();
	UFUNCTION()
	void OnClickedBTNAssassin();
	UFUNCTION()
	void OnClickedBTNWizard();
	UFUNCTION()
	void OnClickedBTNFighter();

	UFUNCTION()
	void OnClickedBTNResultItemDetail();

	UFUNCTION()
	void OnClickedBTNSkipResult();
	UFUNCTION()
	void OnClickedBTNResultConfirm();
	UFUNCTION()
	void OnClickedBTNIncQuantity();
	UFUNCTION()
	void OnClickedBTNDecQuantity();
	UFUNCTION()
	void OnClickedBTNMaxQuantity();
	UFUNCTION()
	void OnClickedBTNExecute();
	UFUNCTION()
	void OnClickedBTNFilter();
	UFUNCTION()
	void OnClickedBTNFilterClose();
	UFUNCTION()
	void OnClickedBTNFilterApply();
	UFUNCTION()
	void OnClickedBTNFilterReset();
	UFUNCTION()
	void OnAnyForgeFilterCBChanged(bool InCheck);

public:
	void OnClickedTabButton(class UB2UIItemForgeTabButton* ButtonWidget);
	void OnClickedForgeList(class UB2UIItemForgeListButton* ButtonWidget);
	void OnSetMaterialID(EItemForgeSlot materialSlot, FB2Item Selecteditem, bool isReady, int32 singleRequiredQuantity, bool thisContainsGear = false, int32 weHaveThatMany = 1);

	void ReadytoShowResult();
	void ContinueShowingResult(EItemForgeResultType previousResult);

private:

	TWeakObjectPtr<UHorizontalBox>		HB_Test;

	TWeakObjectPtr<UB2Button>		BTN_MasterModal;

	TWeakObjectPtr<class UB2UICommonTab> TAB_Forge;
	TWeakObjectPtr<class UB2UICommonTab> TAB_Upgrade;

	TWeakObjectPtr<UB2Button>		BTN_SelectGladiator;
	TWeakObjectPtr<UB2Button>		BTN_SelectAssassin;
	TWeakObjectPtr<UB2Button>		BTN_SelectWizard;
	TWeakObjectPtr<UB2Button>		BTN_SelectFighter;

	TWeakObjectPtr<UHorizontalBox>	HB_TabList;

	TWeakObjectPtr<UOverlay>			O_PleaseSelectForgeStuff;
	TWeakObjectPtr<UTextBlock>		TB_PleaseSelectForgeStuff;
	TWeakObjectPtr<UCanvasPanel>	CP_IveSelectedForgeStuff;

	TWeakObjectPtr<UTextBlock>		TB_ListIsEmpty;

	TWeakObjectPtr<UTextBlock>		TB_AvailableItemList;
	TWeakObjectPtr<UB2ScrollBox>	SB_AvailableItemList;

	TWeakObjectPtr<class UB2UISlotItem> UIP_SelectedItem;

	TWeakObjectPtr<UCanvasPanel>	CP_Right;

	TWeakObjectPtr<UTextBlock>		TB_IngredientList;
	TWeakObjectPtr<UB2ScrollBox>	SB_IngredientList;

	TWeakObjectPtr<UB2Button>		BTN_ResultItemDetail;
	TWeakObjectPtr<class UB2DynItemIcon_ItemForge> UIP_ResultItemIcon;

	TWeakObjectPtr<UTextBlock>		TB_ForgeRate;
	TWeakObjectPtr<UTextBlock>		TB_SuccessRate;

	TWeakObjectPtr<UTextBlock>		TB_Warning_Forge;
	TWeakObjectPtr<UTextBlock>		TB_Warning_Upgrade;

	TWeakObjectPtr<UTextBlock>		TB_ForgeLimit;
	TWeakObjectPtr<UTextBlock>		TB_ForgeLimitNumber;

	TWeakObjectPtr<UHorizontalBox>	HB_ShowingLimitNumber;

	TWeakObjectPtr<UTextBlock>		TB_ForgeQuantity;
	TWeakObjectPtr<UHorizontalBox>	HB_AdjustQuantity;
	TWeakObjectPtr<UTextBlock>		TB_MaximumQuantity;
	TWeakObjectPtr<UB2Button>		BTN_DecQuantity;
	TWeakObjectPtr<USlider>			SD_ControlQuantity;
	TWeakObjectPtr<UTextBlock>		TB_CurrentQuantity;
	TWeakObjectPtr<UB2Button>		BTN_IncQuantity;
	TWeakObjectPtr<UB2Button>		BTN_MaxQuantity;
	TWeakObjectPtr<UTextBlock>		TB_MaxQuantity;

	TWeakObjectPtr<UCanvasPanel>	CP_PeriodLimit;
	TWeakObjectPtr<UB2RichTextBlock> TB_LimitStartTime;	//扁埃 力茄 力累 矫累矫埃
	TWeakObjectPtr<UB2RichTextBlock> TB_LimitEndTime;	//扁埃 力茄 力累 辆丰矫埃

	TWeakObjectPtr<UB2Button>		BTN_Execute;
	TWeakObjectPtr<UTextBlock>		TB_Execute;

	TWeakObjectPtr<UB2Button>		BTN_Filter;
	TWeakObjectPtr<UTextBlock>		TB_Filter;

	TWeakObjectPtr<UTextBlock>		TB_PopupTitle;
	TWeakObjectPtr<UB2UIFilterItemForge> UIP_ForgeFilter;

	TWeakObjectPtr<UCanvasPanel>	CP_Filter;
	TWeakObjectPtr<UB2Button>		F_Btn_Close;
	TWeakObjectPtr<UB2Button>		F_Modal;

	TWeakObjectPtr<UB2Button>		F_BTN_Apply;
	TWeakObjectPtr<UTextBlock>		F_TB_Apply;

	TWeakObjectPtr<UB2Button>		F_BTN_Reset;
	TWeakObjectPtr<UTextBlock>		F_TB_Reset;

	TWeakObjectPtr<UB2Button>		BTN_ItemDetailBackGround;
	TWeakObjectPtr<class UB2UICollectBookItemDetail> UIP_ItemDetail;

	TWeakObjectPtr<UTextBlock>		TB_Price;

	TWeakObjectPtr<UHorizontalBox>	HB_ProgressBar;

	TWeakObjectPtr<UHorizontalBox>	HB_Resource;
	TWeakObjectPtr<class UB2UIHeaderResource> UIP_HeaderResource_DarkCrystal;
	TWeakObjectPtr<class UB2UIHeaderResource> UIP_HeaderResource_Gold;
	TWeakObjectPtr<class UB2UIHeaderResource> UIP_HeaderResource_Gem;

	TWeakObjectPtr<UTextBlock>		TB_Forging;

	TWeakObjectPtr<UCanvasPanel>	CP_Result;
	TWeakObjectPtr<UB2ScrollBox>	SB_ResultScroll;

	TWeakObjectPtr<UTextBlock>		X_TB_ResultTitle;

	TWeakObjectPtr<UB2Button>		X_BTN_Skip;
	TWeakObjectPtr<UTextBlock>		X_TB_Skip;

	TWeakObjectPtr<UTextBlock>		X_TB_Result;
	TWeakObjectPtr<UB2RichTextBlock>		X_TB_Success;
	TWeakObjectPtr<UB2RichTextBlock>		X_TB_HugeSuccess;
	TWeakObjectPtr<UB2RichTextBlock>		X_TB_Fail;

	TWeakObjectPtr<UB2Button>		X_BTN_Confirm;
	TWeakObjectPtr<UTextBlock>		X_TB_Confirm;

protected:
	class FClientDataStore* ClientDataStore;

	class FLocalCharacterData* CachedCharacterStore;
	EItemForgeType selectedForgeType;
	int32 selectedForgeTab;
	EPCClass selectedClass;
	int32 selectedForgeID;
	int32 selectedLimitID;

	bool forgeListRequest = false;

	TArray<FItemForgeInfo>		forgeTable;
	TArray<FItemForgeLimitInfo> LimitStatus;
	TArray<FItemForgePeriodicStatus> PeriodLimitStatus;

	TArray<FItemForgeResult> resultItems;

	UB2UIItemForgeIngredientList* mainIngredientTrack;
	UB2UIItemForgeIngredientList* subIngredientTrack1;
	UB2UIItemForgeIngredientList* subIngredientTrack2;
	UB2UIItemForgeIngredientList* subIngredientTrack3;
	UB2UIItemForgeIngredientList* subIngredientTrack4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UB2UIItemForgeTabButton>	TabButtonBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UB2UIItemForgeListButton>	ListButtonBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UB2UIItemForgeGroupTitle>	GroupTitleBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UB2UIItemForgeIngredientList>	IngredientBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UB2UIItemForgeGroupTitle>	IngredientTitleBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UB2DynItemIcon_ItemForgeResult>	ForgeResultBP;

private:
	FB2Item selectedMainItem;
	//捞霸 4俺肺 绊沥等 捞蜡绰 辑滚率俊辑 4俺肺 绊沥秦 滚啡扁 锭巩
	FB2Item selected1stMaterial;
	FB2Item selected2ndMaterial;
	FB2Item selected3rdMaterial;
	FB2Item selected4thMaterial;

	int32 mainQuantity;
	int32 sub1Quantity;
	int32 sub2Quantity;
	int32 sub3Quantity;
	int32 sub4Quantity;
	int32 lowestQuantity;

	int32 owningMainQuantity;
	int32 owningSub1Quantity;
	int32 owningSub2Quantity;
	int32 owningSub3Quantity;
	int32 owningSub4Quantity;

	int32 leftQualityMain;
	int32 leftQualitySub1;
	int32 leftQualitySub2;
	int32 leftQualitySub3;
	int32 leftQualitySub4;

	int32 resultQuantity;
	int32 bonusQuantity;

	bool mainReady;
	bool sub1Ready;
	bool sub2Ready;
	bool sub3Ready;
	bool sub4Ready;

	bool needMain;
	bool needSub1;
	bool needSub2;
	bool needSub3;
	bool needSub4;

	int32 quantity;
	int32 maxQuantity;	//弥惑窜俊 define等巴 富绊 啊柳 牢亥配府 救俊辑 啊瓷茄 弥措 肮荐
	int32 resultIndex;
	int32 refundIndex;

	FDateTime previewTime;
	FDateTime startTime;
	FDateTime endTime;

	bool forgeAble;

	EPCClass backupClass;

	TArray<FItemForgeInfo> tabArrayForForge;
	TArray<FItemForgeInfo> tabArrayForUpgrade;

	float ElementSize;

	int32 currentLimit;
	bool isUnlimited;

	int32 totalFail;
	int32 totalSuccess;
	int32 totalHugeSuccess;

	bool aniEnd;
	bool dataRecieved;

	bool resultSkip;

	TArray<EItemInvenType> filterInvenType;
	TArray<int32> filterStarGrade;
};