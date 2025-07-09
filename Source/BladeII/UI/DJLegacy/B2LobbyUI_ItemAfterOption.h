// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2LobbyUserWidget.h"
#include "B2Inventory.h"
#include "B2UIBackWidget.h"
#include "B2LobbyUI_ItemAfterOption.generated.h"

DECLARE_DELEGATE(UB2LobbyUI_ItemAfterOption_ClickClose);

UCLASS()
class BLADEII_API UB2LobbyUI_ItemAfterOption : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;

public:
	UB2LobbyUI_ItemAfterOption(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	void DestroySelf() override;
	void UpdateStaticTexts();
	void UpdateOnInfoStateChange();
	void UpdateItemData(FB2Item& InItemData, bool bIsOptionChange);

protected:
	UFUNCTION()
	void OnClickBTN_Close();

	virtual void CloseWidgetDelegate() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemAfterOption")
	TSubclassOf<class UB2LobbyUI_ItemAfterOptionSlot> ItemOptionWidgetClass;

	UB2LobbyUI_ItemAfterOption_ClickClose ClickCloseDelegate;

private:
	TWeakObjectPtr<UTextBlock> STB_OptionTitle;
	TWeakObjectPtr<UTextBlock> STB_Option;

	TWeakObjectPtr<UB2Button> BTN_Close;

	TWeakObjectPtr<UB2ScrollBox> SB_Options;
};

UCLASS()
class BLADEII_API UB2LobbyUI_ItemAfterOptionSlot : public UBladeIIUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void CacheAssets() override;

	virtual void DestroySelf() override;

	void SetSkillInfo(int32 OptionId, float MinValue, float MaxValue, EPCClass PCClass);

private:
	TWeakObjectPtr<UTextBlock> TB_OptionName;
	TWeakObjectPtr<UTextBlock> TB_MinValue;
	TWeakObjectPtr<UTextBlock> TB_MaxValue;

};