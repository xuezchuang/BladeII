// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2UIWidget.h"
#include "B2UIGuildSupportItemPopup.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIGuildSupportItemPopup : public UB2UIWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	void UpdateCharacterTab(EPCClass CharType);
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

	void DestroyAllItemRows();
	void UpdateStoredGuildItemIcons();

	void SetRequestState(bool IsEnabled);

	UFUNCTION()
	void OnCallbackClickIcon(class UB2DynItemIcon_GuildSupport *ItemIcon, int32 TempleteItemID);

	UFUNCTION()
	void OnClickedCharacterTab(EPCClass ClickedHero);

	UFUNCTION()
	void OnClickClose();

	UFUNCTION()
	void OnClickRequestItem();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	TSubclassOf<class UB2UIGuildSupportRowSlot> ItemIconRowWidgetClass;

	UPROPERTY(Transient)
	TArray<class UB2UIGuildSupportRowSlot*> CreatedItemRows;
private:

	TArray<TWeakObjectPtr<class UB2UIGuildSupportCharacterSlot>> CracterTabs;

	TWeakObjectPtr<UB2ScrollBox> SB_Items;
	TWeakObjectPtr<UB2Button> BTN_Close;

	TWeakObjectPtr<UB2Button> BTN_RequestItem;
	TWeakObjectPtr<UTextBlock> TB_RequestItem;

	TWeakObjectPtr<UTextBlock> TB_Title;

	class UB2DynItemIcon_GuildSupport *CachedGuildSupportIcon;

	EPCClass CurrentPCClass;
	uint32	DeliveryAskGuildSupportTicket;
	int32 CachedTempleteID;
};
