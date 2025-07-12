// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "B2LobbyUserWidget.h"
#include "B2Inventory.h"
#include "B2UIBackWidget.h"
#include "../B2RichTextBlock.h"
#include "B2LobbyUI_EnhanceItemSetEffect.generated.h"

/**
 * The first (in fact, the main) step window of Enhnace item feature.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_EnhanceItemSetEffect : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
public:
	UB2LobbyUI_EnhanceItemSetEffect(const FObjectInitializer& ObjectInitializer);
	virtual void CacheAssets() override;
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	void DestroySelf() override;

public:
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnCloseButtonPressed();

private:

	void UpdateStaticTexts();
	void UpdateEnhanceItemSetEffect();
	void UpdateDynamicStaticTexts(EPCClass CurrentPC);

protected:
	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;

	virtual void CloseWidgetDelegate() override;

protected:
	TWeakObjectPtr<UTextBlock>		TB_Title;
	TWeakObjectPtr<UB2RichTextBlock> RTB_SetEffectNotice;
	TWeakObjectPtr<UTextBlock>		TB_EnhanceLevel;
	TWeakObjectPtr<UTextBlock>		TB_NextEnhanceLevel;
	TWeakObjectPtr<UTextBlock>		TB_AttackPoint;
	TWeakObjectPtr<UTextBlock>		TB_NextAttackPoint;
	TWeakObjectPtr<UTextBlock>		TB_DefensePoint;
	TWeakObjectPtr<UTextBlock>		TB_NextDefensePoint;
	TWeakObjectPtr<UTextBlock>		TB_HealthPoint;
	TWeakObjectPtr<UTextBlock>		TB_NextHealthPoint;

	TWeakObjectPtr<UTextBlock>		TB_DeltaAttackPoint;
	TWeakObjectPtr<UTextBlock>		TB_DeltaDefensePoint;
	TWeakObjectPtr<UTextBlock>		TB_DeltaHealthPoint;

	TWeakObjectPtr<UTextBlock>		STB_EnhanceLevel;
	TWeakObjectPtr<UTextBlock>		STB_CurrentValue;
	TWeakObjectPtr<UTextBlock>		STB_NextValue;
	TWeakObjectPtr<UTextBlock>		STB_Attck;
	TWeakObjectPtr<UTextBlock>		STB_Defense;
	TWeakObjectPtr<UTextBlock>		STB_Health;

	TWeakObjectPtr<UCanvasPanel>	CP_MaxEnhanceLevel;
	TWeakObjectPtr<UTextBlock>		TB_MaxEnhanceLevelDesc;
};
