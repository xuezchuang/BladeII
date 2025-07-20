// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "B2UICollectBookItemDetail.generated.h"

DECLARE_DELEGATE(FOnClickDetailClose);

UCLASS()
class BLADEII_API UB2UICollectBookItemDetail : public UB2UIWidgetBase, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	class UB2UICollectBookDetailPart* CreateDetailPart(const FText& DetailPartTitle);

	void SetDetailViewItemLevel(int32 CurrentLevel, int32 MaxLevel);
	void SetDetailViewItemEnhance(int32 CurrentEnhance, int32 MaxEnhance);
	void SetDetailViewSingleItem(int32 ItemTemplateID, EPCClass ClassType, const int32 SurpassLevel = 0, FB2Item OriginalItem=NULL);

	void SetDetailViewSetAbility(int32 SetUniqueKey, EPCClass ClassType);
	void SetDetailViewSetComposition(int32 SetUniqueKey, EPCClass ClassType);

	void SetItemIconDefaultTemplate(class UB2UICollectBookIcon* ItemIconWidget);	
	void SetItemIconDefaultTemplate(int32 ItemGrade, UMaterialInterface* BGMaterial, EItemCollectionState ItemCollectionState);
	void SetItemDisplayInfo(const FText& ItemName, UMaterialInterface* IconMtrl);

	void SetItemPrimaryOption(EItemPrimaryPointType PrimaryOptionType, float MinValue, float MaxValue);
	void SetItemIconStarGrade(const int32 ItemGrade, const bool IsSurpass);

	void SetDetailViewCloseDelegate(const FOnClickDetailClose& Delegate);
	void SetCollectBookIcon(EItemClass ItemClass, int32 RefID);
	void SetSelectiveLottery();
	void SetEnableSlotEffect(bool bVisible);
	void SetEnableBackGround(bool bVisible);
	void OpenPopup();

private:

	void DestroyDetailParts();

	void ShowItemLevelControl(bool bVisible);
	void OnChangedLevel(int32 NewLevel);
	void OnChangedEnhance(int32 NewEnhance);
	
protected:

	int32 MaxLevel;
	int32 CurrentLevelupFactor;

	int32 MaxEnhance;
	int32 CurrentEnhanceFactor;

	EItemPrimaryPointType PrimaryPointType;
	float CurrentItemLevel;
	int32 CurrentItemEnhance;
	int32 ItemOptionCombatValue;

	TArray<class UB2UICollectBookDetailPart*> DetailPartWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	TSubclassOf<class UB2UICollectBookDetailPart> DetailPartClass;

	FOnClickDetailClose DetailCloseDelegate;

	int32 CachedSurpassLevel;
	FB2Item CachedItem;
	
protected:
	UFUNCTION()
	void OnClickBTN_LevelDown();
	UFUNCTION()
	void OnClickBTN_LevelUp();
	UFUNCTION()
	void OnClickBTN_EnhanceDown();
	UFUNCTION()
	void OnClickBTN_EnhanceUp();
	UFUNCTION()
	void OnClickBTN_Close();
	UFUNCTION()
	void OnChangeLevelSlider(float InValue);
	UFUNCTION()
	void OnChangeEnhanceSlider(float InValue);


protected:

	TWeakObjectPtr<UVerticalBox> VB_DetailParts;

	TWeakObjectPtr<UCanvasPanel> CP_LevelControl;
	TWeakObjectPtr<class UB2UICollectBookIcon> UIP_CollectBookIcon;
	TWeakObjectPtr<UB2Button> BTN_LevelDown;
	TWeakObjectPtr<UB2Button> BTN_LevelUp;
	TWeakObjectPtr<UTextBlock> TB_EquipmentEnhance;
	TWeakObjectPtr<UTextBlock> TB_Level;
	TWeakObjectPtr<UTextBlock> TB_Enhance;
	TWeakObjectPtr<UTextBlock> TB_PrimaryOptionName;
	TWeakObjectPtr<UTextBlock> TB_PrimaryOptionValue;
	TWeakObjectPtr<UTextBlock> TB_CombatName;
	TWeakObjectPtr<UTextBlock> TB_CombatValue;
	TWeakObjectPtr<UTextBlock> TB_EquipmentLv;

	TWeakObjectPtr<USlider>		SD_Level;	

	TWeakObjectPtr<UImage>		 IMG_BackGround;
	TWeakObjectPtr<UTextBlock>   TB_ItemName;
	TWeakObjectPtr<UB2Button>    BTN_Close;
	TWeakObjectPtr<UPanelWidget> P_LevelSlide;
	TWeakObjectPtr<UPanelWidget> P_EnhanceSlide;

	TWeakObjectPtr<USlider>	  SD_Enhance;
	TWeakObjectPtr<UB2Button> BTN_EnhanceDown;
	TWeakObjectPtr<UB2Button> BTN_EnhanceUp;
};



UCLASS()
class BLADEII_API UB2UICollectBookDetailPart : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetTitleText(const FText& TitleText);
	void HideTitleText();

	void AddItemOptionTextLine(EItemOption OptionType, EPCClass ClassType, float Value);
	void AddSubTitleLine(const FText& SubTitleText);
	void AddRandOptionTextLine();
	void AddSealTextLine();

	void AddSetEffectTextLine(int32 SetCount, EItemOption OptionType, EPCClass ClassType, float IncreaseValue);
	void AddSetCompositionTextLine(int32 ItemTemplateID, bool bHasItem);

	void DestroyTextLines();

	class UB2DynText_Multiple* CreateTextLine(const TSubclassOf<class UB2DynText_Multiple>& TextClass);

protected:

	// 2 TextBlock (Left , Right)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	TSubclassOf<class UB2DynText_Multiple> DefaultLineTextClass;

	// 1 TextBlock ( Middle )
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	TSubclassOf<class UB2DynText_Multiple> SingleLineTextClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
	TSubclassOf<class UB2DynText_Multiple> SubTitleTextClass;

private:

	TWeakObjectPtr<UOverlay> O_PartTitle;
	TWeakObjectPtr<UTextBlock> TB_HeaderTitle;

	TWeakObjectPtr<UVerticalBox> VB_DetailOptions;
	

private:

	TArray<class UBladeIIUserWidget*>	CachedTextLines;

};