// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "UI/DJLegacy/B2LobbyUserWidget.h"
#include "B2UIBackWidget.h"
#include "B2UILobby_CostumeMain.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UILobby_CostumeMain : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	virtual void DestroySelf() override;

	virtual void DoMarkRedDot() final;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void CloseWidgetDelegate() override;

private:
	void CostumeMain_CacheAssets();
	void CostumeMain_BindDelegates();
	void CostumeMain_UpdateStaticText();

private:
	void SubscribeEvents();
	void UnsubscribeEvents();

private:
	void InitCostumeMain();
	void InitCostumePage();
	void ReleaseCostumeMain();

private:
	void UpdateCostumePage(const ECostumePage& eCostumePageType);
	void UpdateCostumePageValidity(const int32& iCostumePageIndex);
	void UpdateCostumePagesVisibility(const int32& iVisiblePageIndex);

private:
	void InitInputDelay();
	void UpdateInputDelay(const float& InDeltaTime);
	bool IsEnableInputDelay();

private:
	void CloseAllStandaloneUIFromUIManager();

private:
	UB2UIWidget* CreateDynamicCostumePage(const TSoftClassPtr<UB2UIWidget>* pCreateWidgetClass);

public:
	UCanvasPanel* GetDetailPopupParentPanel(bool bOfLeftSide) const;

private:
	UFUNCTION()
	void OnClickBTN_OpenInvenPage();

	UFUNCTION()
	void OnClickBTN_OpenStorePage();

	UFUNCTION()
	bool RedDotCondition_Inven();
	
protected:
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite)
	class UB2LobbyUI_HeroMgmtCommon* HeroMgmtBaseNRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftClassPtr<class UB2UIWidget>>	CostumePageSubClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float fMaxInputDelay;

private:
	TWeakObjectPtr<UCanvasPanel>		CP_CostumePage;

	TWeakObjectPtr<UCanvasPanel>		X_LeftPopupPanel;
	TWeakObjectPtr<UCanvasPanel>		X_RightPopupPanel;

	//WidgetSwitcher
	TWeakObjectPtr<UWidgetSwitcher>		WS_InvenOrStoreOpenBtn;

	TWeakObjectPtr<UB2Button>			BTN_OpenInvenPage;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_OpenInvenPageBtn;

	TWeakObjectPtr<UB2Button>			BTN_OpenStorePage;
	TWeakObjectPtr<UB2RichTextBlock>	RTB_OpenStorePageBtn;
	//WidgetSwitcher

private:
	TArray<TWeakObjectPtr<class UB2UIWidget>>	ArrayCostumePage;

	float										fCurrentInputDelay;

	ECostumePage								FirstInPage;

private:
	TArray<issue_ptr>	Issues;
};
