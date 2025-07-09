// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "BladeIIPlayer.h"
#include "B2UIBackWidget.h"
#include "B2UIUtil.h"
#include "B2LobbyUI_ExtendInventory.generated.h"


/**
 * One of Lobby inventory's pop-up. Extending (or purchasing) inventory.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_ExtendInventory : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
protected:
	virtual void CloseWidgetDelegate() override;

	void BindDelegates();
	void UpdateStaticText();

	//////////////////////////////////////////////////
	
	TWeakObjectPtr<UTextBlock> TB_Title;
	
	TWeakObjectPtr<UTextBlock> TB_CurrentLabel;
	TWeakObjectPtr<UTextBlock> TB_PostExtendLabel;

	TWeakObjectPtr<UTextBlock> TB_CurrentNumber;
	TWeakObjectPtr<UTextBlock> TB_PostExtendNumber;

	TWeakObjectPtr<UTextBlock> TB_MaxSlotCountInfo;
	
	TWeakObjectPtr<UTextBlock> TB_ExtendBtn;
	TWeakObjectPtr<UTextBlock> TB_ExtendCost;

	TWeakObjectPtr<UB2Button>	BTN_Down;
	TWeakObjectPtr<UB2Button>	BTN_Up;

	TWeakObjectPtr<UB2Button>	BTN_MaxExtend;
	TWeakObjectPtr<UTextBlock>	TB_MaxExtend;

	TWeakObjectPtr<USlider>		S_CashExtendSlider;

	TWeakObjectPtr<UB2Button>	BTN_Close;
	TWeakObjectPtr<UB2Button>	BTN_Extend;
	
	TWeakObjectPtr<UImage>		IMG_Portrait;
	
	//////////////////////////////////////////////////
	EPCClass CurrSelectedPCClass;
	EItemInvenType CurrSelectedInvenType;

	int32 CashPerSingleExtend;

	B2SlotSliderLogic SlotSliderLogic;
	
	UPROPERTY(EditAnywhere)
	TArray<FB2ClassPortraitImage> PortraitInfo;

public:
	
	UB2LobbyUI_ExtendInventory(const FObjectInitializer& ObjectInitializer);

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	virtual void CacheAssets() override;

	/** Like that of B2UIWidget.. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnOpen_BP();

private:
	void UpdateWidgets(int32 NewExtendCount);

	void CalculateData();
	int32 GetSliderExtendCount();
	int32 GetRealExtendCount();

	UFUNCTION()
	void OnClickBtnDown();
	UFUNCTION()
	void OnClickBtnUp();
	UFUNCTION()
	void OnChangedExtendCount(float ChangeValue);

	UFUNCTION()
	void OnClickBtnClose();

	UFUNCTION()
	void OnClickBtnExtend();

	UFUNCTION()
	void OnClickBtnMaxExtend();


	void CloseMe();
};
