// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2UIBackWidget.h"
#include <functional>
#include "CommonStruct.h"
#include "B2AcqItemEquipSuggest.generated.h"

typedef std::function<void(void)> B2AcqItemEquipSuggestCBType;

/**
 * Kinda like message pop-up, specially for acquired item equip suggestion.
 */
UCLASS()
class BLADEII_API UB2AcqItemEquipSuggest : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CloseWidgetDelegate();

	FB2Item EquipSuggestedItem;

	TWeakObjectPtr<UCanvasPanel> CP_ItemIcon;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_PrimPointType;
	TWeakObjectPtr<UTextBlock> TB_PrimPoint;
	//TWeakObjectPtr<UTextBlock> TB_IncAmount;
	TWeakObjectPtr<UTextBlock> TB_ItemName;
	TWeakObjectPtr<UTextBlock> TB_Equip;

	B2AcqItemEquipSuggestCBType OnCloseCB; // We got some stuff to be done continuously until all checks are done.

	//////////////////////////////////////////////////////////////////////
	// For item icon

	/** Widget class for a single item icon. It will be dynamically created inside. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemIcon")
	TSubclassOf<class UB2DynItemIcon_LobbyInven> ItemIconWidgetClass;

	/** Item icon created inside CP_ItemIcon */
	UPROPERTY(Transient)
	class UB2DynItemIcon_LobbyInven* CreatedItemIcon;

	//////////////////////////////////////////////////////////////////////
	
	TWeakObjectPtr<UB2Button> BTN_Equip;
	TWeakObjectPtr<UB2Button> BTN_Close;

public:

	UB2AcqItemEquipSuggest(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;
	virtual void DestroySelf() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	void UpdateStaticTexts();

	void UpdateData(bool bWasEqiupped, const FB2Item& CurrentEquippedItem, const FB2Item& InSuggestedItem, B2AcqItemEquipSuggestCBType InOnCloseCB);

	UFUNCTION()
	void OnClickedEquipBtn();

	UFUNCTION()
	void OnClickedCloseBtn();

private:

	void CloseMe();

	void DestroyItemIcon();
};
