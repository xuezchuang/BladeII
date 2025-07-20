// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2UICollectBookWing.generated.h"

DECLARE_DELEGATE_OneParam(FOnClickedCollectionWingGrade, int32);
DECLARE_DELEGATE(FOnClickedWingOptionPanel);

UCLASS()
class BLADEII_API UB2UICollectBookWingOption : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void UpdateStaticText() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;


	void InitPropIndex(int32 Index) { PropIndex = Index; }
	void SetWingOption(const FWingPropOption* OptionPtr, EPCClass InCharacterClass);
	void UpdateWingOption(const FWingPropOption& WingOption);

	const int32 GetPropIndex() const { return PropIndex; }
	const FWingPropOption& GetCurrentPropOption() const { return PropOption; }

	UFUNCTION()
	void SetSlider(float InValue);

private:

	int32			PropIndex;
	EPCClass		CharacterClass;
	FWingPropOption	PropOption;
	bool			bInitPropOption;

private:

	UFUNCTION()
	void OnClickBTN_LevelDown();
	UFUNCTION()
	void OnClickBTN_LevelUp();

	UFUNCTION()
	void OnClickBTN_LevelMax();

private:

	TWeakObjectPtr<class UB2WingUIP_PropOptionIcon> UIP_WingOptIcon;
	TWeakObjectPtr<UTextBlock>	TB_OptionName;
	TWeakObjectPtr<UTextBlock>	TB_OptionValue;

	TWeakObjectPtr<UCanvasPanel> CP_LevelControl;
	TWeakObjectPtr<UButton>		BTN_LevelDown;
	TWeakObjectPtr<UButton>		BTN_LevelUp;
	TWeakObjectPtr<UButton>		BTN_Max;

	TWeakObjectPtr<USlider>		S_OptionLevelSlider;

	TWeakObjectPtr<UTextBlock>	TB_CurrentLevel;
	TWeakObjectPtr<UTextBlock>  TB_MaxLevelDesc;
	TWeakObjectPtr<UTextBlock>	TB_MaxLevel;
	TWeakObjectPtr<UTextBlock>	TB_EnhanceLevel;
	

	TWeakObjectPtr<UB2RichTextBlock>	TB_OptionLock;
};

UCLASS()
class BLADEII_API UB2UICollectBookWingOptionPanel : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;
	virtual void UpdateStaticText() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	
	void SetPropInfo(EPCClass CharacterClass, int32 PropIndex, const FWingPropOption& PropOption);
	void SetOwnerWingUI(class UB2UICollectBookWing* InWingUIPtr)
	{
		OwnerWingUI = InWingUIPtr;
	}

	void OpenPopup();
protected:

	UFUNCTION()
	void OnClickBTN_Close();

protected:

	UB2UICollectBookWing* OwnerWingUI;

	TWeakObjectPtr<UTextBlock>	TB_WingOptionTitle;
	TWeakObjectPtr<UTextBlock>	TB_WingOptionDesc;
	TWeakObjectPtr<UButton>		BTN_Close;

	TArray<TWeakObjectPtr<UB2UICollectBookWingOption>>	WingOptionSlots;
};


struct FWingGradeTab
{
	FWingGradeTab() : BTN_Area(nullptr), O_GradeOverlay(nullptr), TB_WingGrade(nullptr) {}

	/*TWeakObjectPtr<UImage> IMG_Selected;
	TWeakObjectPtr<UImage> IMG_UnSelected;

	TWeakObjectPtr<UButton> BTN_Area;
	TWeakObjectPtr<UTextBlock> TB_WingGrade;

	void SetSelected(bool bSelected, const FLinearColor& TextColor);*/

	////////////////////////////////////////// new Tab

	TWeakObjectPtr<UButton>		BTN_Area;
	TWeakObjectPtr<UOverlay>	O_GradeOverlay;
	TWeakObjectPtr<UTextBlock>	TB_WingGrade;

};

UCLASS()
class BLADEII_API UB2UICollectBookWing : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	
	virtual void UpdateStaticText() override;
public:

	UB2UICollectBookWing(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetCharacterClass(EPCClass InCharacterClass) { CharacterClass = InCharacterClass; }
	void OnClickedWingPage();

	void SetClickWingGradeDelegate(FOnClickedCollectionWingGrade& Delegate);
	void SetClickWingOptionDelegate(FOnClickedWingOptionPanel& Delegate);

	void OverrideWingPropOption(int32 PropIndex, const FWingPropOption& PropOption);
	void OnClickedWingGrade(int32 WingGrade) { UpdateWingPage(WingGrade); }

private:

	void UpdateWingPage(int32 WingGrade, bool bForce = false);

	void UpdateWingLevelVisible(int32 WingLevel);
	void UpdateWingAbility(int32 WingGrade, int32 WingLevel);
	void UpdatePrimaryAbility(int32 Attack, int32 Defence, int32 Health);

protected:

	int32		CurrentWingGrade;
	int32		CurrentEnhanceLevel;
	FB2Wing		WingInfo;

	EPCClass	CharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor UnselectedWingGradeTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor SelectedWingGradeTextColor;

protected:

	UFUNCTION()
	void OnClickBTN_BeforeWingGrade() { UpdateWingPage(CurrentWingGrade - 1); }
	UFUNCTION()
	void OnClickBTN_WingGrade() { UpdateWingPage(CurrentWingGrade); }
	UFUNCTION()
	void OnClickBTN_AfterWingGrade() { UpdateWingPage(CurrentWingGrade + 1); }

	UFUNCTION()
	void OnClickBTN_WingLevelUp();
	UFUNCTION()
	void OnClickBTN_WingLevelDown();

	UFUNCTION()
	void OnClickBTN_WingOptionPanel();

private:
	TWeakObjectPtr<UHorizontalBox> HB_WingGradeTabs;
	FWingGradeTab				BeforeTab;
	FWingGradeTab				SelectTab;
	FWingGradeTab				AfterTab;
	TWeakObjectPtr<UTextBlock> TB_UpperWingName;
	TWeakObjectPtr<UTextBlock> TB_AttackIncLabel;
	TWeakObjectPtr<UTextBlock> TB_AttackIncAmount;
	TWeakObjectPtr<UTextBlock> TB_DefenseIncLabel;
	TWeakObjectPtr<UTextBlock> TB_DefenseIncAmount;
	TWeakObjectPtr<UTextBlock> TB_HealthIncLabel;
	TWeakObjectPtr<UTextBlock> TB_HealthIncAmount;

	TWeakObjectPtr<UImage> IMG_BigFatIcon;
	TWeakObjectPtr<class UB2WingUIP_EvolveGradeStar> UIP_WingGradeStar;

	TWeakObjectPtr<UOverlay> O_WingLevel;
	TWeakObjectPtr<UButton> BTN_WingLevelUp;
	TWeakObjectPtr<UButton> BTN_WingLevelDown;
	TArray<TWeakObjectPtr<class UB2WingUIP_EnhLevelProgElem>> UIP_WingLevelIcons;

	TArray<TWeakObjectPtr<class UB2WingUIP_PropOptionIcon>> UIP_WingOptionIcons;
	TArray < TWeakObjectPtr<UButton>>						BTN_WingOptionIcons;
	TArray<TWeakObjectPtr<UB2UICollectBookWingOption>>	UIP_WingOptions;

	TWeakObjectPtr<UButton>	BTN_WingOptionPanel;

	TWeakObjectPtr<UTextBlock>	TB_WingOption;

	FOnClickedCollectionWingGrade	WingGradeClickDelegate;
	FOnClickedWingOptionPanel		WingOptionClickDelegate;

	UB2UICollectBookWingOptionPanel*	WingOptionPanelWidget;
};


