// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "B2UIBackWidget.h"
#include "EventSubsystem.h"
#include "../Widget/B2ButtonGoodInfoToolTip.h"
#include "B2LobbyUI_ConvertItemPopup.generated.h"

DECLARE_DELEGATE(FConvertItemPopupOnClick)

/**
 *  해당 UI는 재화 아이템을 변환시켜서 다른 재화로 만들어 주는 시스템의 UI입니다.
 *	기존 소스의 활용으로 인해 승급석 외엔 다른 아이템의 변환이 불가능합니다.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_ConvertItemPopup : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	FB2Item* ItemBeforeConvert; // Would not be valid when bIsForSellAllResult is true.
	FB2Item* ItemAfterConvert; // Would not be valid when bIsForSellAllResult is true.
	
protected:
	virtual void CloseWidgetDelegate() override;
	class FClientDataStore *CachedDataStore;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_AskForConverting;

	TWeakObjectPtr<UTextBlock> TB_BeforeConvertQuantity;
	TWeakObjectPtr<UTextBlock> TB_AfterConvertQuantity;

	TWeakObjectPtr<UImage> IMG_BeforeConvert;
	TWeakObjectPtr<UImage> IMG_AfterConvert;
	TWeakObjectPtr<UImage> IMG_Arrow;
	TWeakObjectPtr<UB2ButtonGoodInfoToolTip> BTN_BeforeConvertInfo;
	TWeakObjectPtr<UB2ButtonGoodInfoToolTip> BTN_AfterConvertInfo;

	TWeakObjectPtr<UImage> IMG_Gold;
	TWeakObjectPtr<UImage> IMG_Crystal;
	TWeakObjectPtr<UTextBlock> TB_GoldPrice;
	TWeakObjectPtr<UTextBlock> TB_CrystalPrice;

	TWeakObjectPtr<UTextBlock> TB_GoldConvertLimit;
	TWeakObjectPtr<UTextBlock> TB_MaxGoldConvertCount;
	TWeakObjectPtr<UTextBlock> TB_CrystalConvertLimit;
	TWeakObjectPtr<UTextBlock> TB_MaxCrystalConvertCount;

	TWeakObjectPtr<UB2Button> BTN_PayGold;
	TWeakObjectPtr<UB2Button> BTN_PayCrystal;
	TWeakObjectPtr<UB2Button> BTN_Cancel;

	TWeakObjectPtr<UVerticalBox> V_Content;

public:
	UB2LobbyUI_ConvertItemPopup(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SetRemainTime();

	void PopupMain(int32 convertItemID, int32 convertGold, int32 convertCrystal, const FConvertItemPopupOnClick& InCancelBtnHandler);
	void PopupAsConvertResult(const int32 AcquiredGold);

	void SetConvertInfo(int32 beforeConvertItem, int32 goldConvertCount, int32 crystalConvertCount);

	void SetGoldCost(EStoreItemCost InCostType, int32 price, int32 convertCount);
	void SetCrystalCost(EStoreItemCost InCostType, int32 price, int32 convertCount);

	void SetGoldCountInfo(int32 LastCount, int32 MaxCount);
	void SetCrystalCountInfo(int32 LastCount, int32 MaxCount);

	void ResponseConvertItem(int32 itemid);

private:
	void UpdateStaticWidgets();
	void UpdateDynamicWidgets();
	void UpdateConvertItemWidgets();

	template<class T>
	void ResponseConvertData(const T& ConvertDataPtr);

	void SetConvertTableData(int32 beforeConvertItemID_, int32 goldConvertCount, int32 crystalConvertCount);

private:
	UFUNCTION()
		void OnClickPayGoldBtn();
	UFUNCTION()
		void OnClickPayCrystalBtn();

	UFUNCTION()
		void OnClickCancelBtn();

public:
	void CloseMe();
	void UpdateRemainTime();

	int32 beforeconvertItemID;
	int32 beforeconvertItemID_ref;
	int32 afterconvertItemID;
	int32 beforeConvertItemQuantity;
	int32 requiredBeforeConvertItemQuantity;
	int32 resultAfterConvertItemQuantity;

private:
	FConvertItemPopupOnClick OnClickCancelBtnHandler;
	FDateTime NextRenewalTime;
	bool StartTimeTick;

	

	int32 goldPrice;
	int32 crystalPrice;

	int32 tableID;

	float ElementSize;
};
