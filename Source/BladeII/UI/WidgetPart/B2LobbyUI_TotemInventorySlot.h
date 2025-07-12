// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "../../DataStore/B2TotemData.h"
#include "../Widget/B2ScrollBox.h"
#include "../B2UnitedWidgetBase.h"
#include "../../BladeII/BladeIIGameMode.h"
#include "B2LobbyUI_TotemInventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2LobbyUI_TotemInventorySlot : public UB2UIWidget
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	
private:
	void SubscribeEvents();
	void UnsubscribeEvents();

protected:
	virtual void OnSetupManualScrollBoxSender(class UB2UnitedWidgetBase* InReceiver, class UB2ScrollBox* HandlingScrollBox) override;
	virtual void OnClickedSenderBTNArea() override;

private:
	UFUNCTION()
		void OnClickBTN_TotemInventorySlot();

public:
	void UpdateTotemSlot_FromFB2Totem(const FB2Totem& InTotem);
	void SetIsOpenItemInfoPopup(bool OpenInfoPopup);

	class UMaterialInstanceConstant* GetBGPanelMIC();
	class UMaterialInstanceConstant* GetMainIconMIC();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void PlayClickSound();

private:
	void UpdateLockState(const bool& LockState);
	void UpdateIsSmelting(const FB2Totem& InTotem);
	void UpdateSmeltingLevel(const uint8& SmeltingLevel);

	void UpdateIMGTotemIcon(class UB2ItemInfo* pAllItemInfo, struct FSingleItemInfoData* pItemInfo);
	void UpdateIMGTotemGrade(const uint8& Grade);
	void UpdateTBTotemName(struct FSingleItemInfoData* pItemInfo, const uint8& Grade);
	void UpdateEquipMark(const FB2Totem& InTotem);
	void UpdateRedDot(const FB2Totem& InTotem);

	// 치트키용 ID 확인 ////////////////////////////
public:
	bool ShouldShowDevUI() const;

	FText GetItemRefId() const;
	FText GetItemUId() const;

	UFUNCTION()
		void OnPressedBTNItemUid();
	UFUNCTION()
		void OnPressedBTNItemRefid();
	////////////////////////////////////////////////

private:
	void OpenTotemInfoPopup(const FB2Totem& InTotem, const bool& IsLeftPopup = false);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsOpenTotemInfoPopup;

private:
	TWeakObjectPtr<UImage>				IMG_TotemIcon;
	TWeakObjectPtr<UImage>				IMG_TotemGrade;
	TWeakObjectPtr<UTextBlock>			TB_TotemName;
	TWeakObjectPtr<UTextBlock>			TB_SmeltingLevel;

	TWeakObjectPtr<UB2Button>			BTN_TotemInventorySlot;

	TWeakObjectPtr<UImage>				IMG_LockState;
	TWeakObjectPtr<UOverlay>			O_EquipMark;
	TWeakObjectPtr<UTextBlock>			TB_EquipMark;

	TWeakObjectPtr<UWidgetSwitcher>		WS_SmeltingInfo;

	TWeakObjectPtr<UOverlay>			O_IsSmelting;
	TWeakObjectPtr<UTextBlock>			TB_IsSmelting;

	TWeakObjectPtr<UTextBlock>			TB_Option0;
	TWeakObjectPtr<UTextBlock>			TB_Option1;
	TWeakObjectPtr<UTextBlock>			TB_Option2;

	TWeakObjectPtr<class UB2UIWidgetBase>	UIP_RedDot;

	// 치트키용 ID 확인 ////////////////////////////
	TWeakObjectPtr<UPanelWidget> P_DevUI;
#if !UE_BUILD_SHIPPING
	TWeakObjectPtr<UTextBlock> TB_ItemRefId_Dev;
	TWeakObjectPtr<UTextBlock> TB_ItemUId_Dev;
	TWeakObjectPtr<UB2Button> BTN_ItemRefId_Dev;
	TWeakObjectPtr<UB2Button> BTN_ItemUId_Dev;
#endif
	////////////////////////////////////////////////

private:
	TArray<issue_ptr> Issues;

	FB2Totem CachedB2TotemInfo;
};
