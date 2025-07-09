// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "BladeIIPlayer.h"
#include "CommonStruct.h"
#include "B2LobbyUI_CharStatDetailPopup.generated.h"

DECLARE_DELEGATE(FDelegateCharStatDetailClose);


/**
 * Displaying a player character's various stats influenced by item equipment.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_CharStatDetailPopup : public UB2LobbyUserWidget
{
	GENERATED_BODY()

protected:	
	TWeakObjectPtr<UVerticalBox>	VB_Stats;
	TWeakObjectPtr<UTextBlock>		TB_StatDetailTitle;
	TWeakObjectPtr<UB2Button>		BTN_Close;
	TWeakObjectPtr<UCanvasPanel>	CP_Detail;

protected:
	int32							BasePopupHeight;

	UPROPERTY(EditAnywhere, Category = "ItemOptions")
	int32							StatSlotOffSet;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemOptions")
	TSubclassOf<class UB2DynText_Multiple> StatTextDisplayClass;

	UPROPERTY(Transient)
	TArray<class UB2DynText_Multiple*>	CreatedStatTextBlocks;

public:
	FDelegateCharStatDetailClose	OnClose;

public:
	UB2LobbyUI_CharStatDetailPopup(const FObjectInitializer& ObjectInitializer);
	
	virtual void CacheAssets() override;
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	virtual void DestroySelf() override;
	void UpdateStaticTexts();

	/** Main update function here, to update all displaying texts. */
	void UpdateStats(EPCClass PCClassToShow, EUnitedCombatOptions CombatOption, bool bFindAccountInfo = false);
	void UpdatePosition(FVector2D position);

private:
	int32 CreateStats(class UVerticalBox* VBToCreateIn, EUnitedCombatOptions InUnitedOptionType, EPCClass InDisplayPCClas, bool bFindAccountInfo);	
	int32 CreateStatDynText(class UVerticalBox* VBToCreateIn, int32 RowIndexInVB, const FText& InStatLabelText, const FText& InStatValueText);
	int32 CreateStatDynText(class UVerticalBox* VBToCreateIn, int32 RowIndexInVB, const FText& InStatLabelText, int32 InStatValue);
	int32 CreateItemOptionStat(class UVerticalBox* VBToCreateIn, int32 RowIndexInVB, ECharStatApplyType InPrimaryPointApplyType, EUnitedCombatOptions InUnitedOptionType, EPCClass InPCClass, float EffectiveValue);
	int32 CreateGuildOptionStat(class UVerticalBox* VBToCreateIn, int32 RowIndexInVB, FText InGuildText,EUnitedCombatOptions InUnitedOptionType, float EffectiveValue);

	void UpdateWidgetSize(int32 StatSlotNum);
	void AdjustWidgetPositionForViewPortSize();
public:
	void DestroyStatTexts();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnCloseButtonClicked();
};
