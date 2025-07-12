// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "../B2RichTextBlock.h"
#include "../Widget/B2Button.h"
#include "B2UIEtherOption.generated.h"

/**
 * 
 */


struct FEtherOption
{
	TWeakObjectPtr<UB2RichTextBlock>							RTB_DefenceSetInfo;
	TWeakObjectPtr<UPanelWidget>								PW_DefenceSet;
	TWeakObjectPtr<UB2Button>									BTN_Select;
	TWeakObjectPtr<UB2Button>									BTN_Info;
	TWeakObjectPtr<UB2Image>									IMG_Check;
	TWeakObjectPtr<UB2Image>									IMG_UnCheck;
	int32														EtherSetID;
	EPCClass													PCClass;

	void Init();
	void SetInfo(int32 InSetID, EPCClass InPCClass);
	void MarkCheckBox();
	void OnClick();
	void SetEnableSetOption(bool InEnable);
	bool IsChecked();
	FText GetEtherSetOptionColorText(int32 EtherSetId, bool IsFadeIn);
};

UCLASS()
class BLADEII_API UB2UIEtherOption : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ether")
		TSubclassOf<class UB2EtherScrollOptionSlot> EtherOptionSlotClass;

	void SetInfo(EPCClass InPCClass);
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

private:
	const int MAX_DEFENCE_SET = 3;

	UFUNCTION()
		void OnClickFirstDefInfo();
	UFUNCTION()
		void OnClickSecondDefInfo();
	UFUNCTION()
		void OnClickThirdDefInfo();

	UFUNCTION()
		void OnClickAttSetInfo();
	
	UFUNCTION()
		void OnClickFirstDefSetInfo();
	UFUNCTION()
		void OnClickSecondDefSetInfo();
	UFUNCTION()
		void OnClickThirdDefSetInfo();
	
	UFUNCTION()
		void OnClickHelp();

	void UpdateDefenseInfo();
	void SetEnableCheckText(const FEtherOption& InEtherOption);
	

	TWeakObjectPtr<UB2RichTextBlock>									X_RTB_Title;
	TWeakObjectPtr<UB2RichTextBlock>									X_RTB_SetOption;
	TWeakObjectPtr<UB2RichTextBlock>									X_RTB_OffenseSet;
	TWeakObjectPtr<UB2RichTextBlock>									X_RTB_OffenseSetInfo;
	TWeakObjectPtr<UB2Button>											X_BTN_OffenseSetInfo;
	TWeakObjectPtr<UB2Button>											X_BTN_Help;
	TWeakObjectPtr<UCheckBox>											X_CB_OffenseSetInfo;
	TWeakObjectPtr<UHorizontalBox>									X_H_OffenseSet;
	TWeakObjectPtr<UB2RichTextBlock>									X_RTB_DefenceSet;
	TWeakObjectPtr<UB2RichTextBlock>									X_RTB_NormalOption;
	TWeakObjectPtr<UB2ScrollBox>										X_SB_OptionScroll;
	TArray<TWeakObjectPtr<class UB2EtherScrollOptionSlot>>				UIP_EtherOptionSlots;
	TArray<FEtherOption>												EtherOptions;
	EPCClass															PCClass;
	uint32																SelectEtherDefenseOptionEventID;
	int32																OffenseID;
};
