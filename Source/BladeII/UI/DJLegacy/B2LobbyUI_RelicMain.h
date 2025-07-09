// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UObject/Object.h"
#include "B2LobbyUserWidget.h"
#include "B2Airport.h"
#include "RelicManager.h"
#include "B2LobbyUI_RelicMain.generated.h"


/**
 * Main Relic management page user widget.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_RelicMain : public UB2LobbyUserWidget
{
	GENERATED_BODY()
	
	/** Source of displayed information here. */
	UPROPERTY(Transient)
	class UB2Airport* CachedAirport;
	EPCClass CurrentPCClass;

	// BluePrint Datas
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII RelicUI")
	TSubclassOf<class UB2RelicUIP_EnchantInfo> RelicItemSlotClass;

protected:
	static class AB2LobbyGameMode* TransientLobbyGM;

	/** Contains common part of several hero management UI pages. */
	TWeakObjectPtr<class UB2LobbyUI_HeroMgmtCommon> UIP_HeroMgmtBase;
	/** Containing wing name, evolution grade, prop option icons */

	TWeakObjectPtr<UOverlay> OV_RelicMenuEnchant;

	TWeakObjectPtr<UOverlay> OV_CancelEnchantRelic;
	TWeakObjectPtr<UHorizontalBox> HB_EnchantRelic;

	// After Renew
	TWeakObjectPtr<UGridPanel>  X_RelicItemList;

	TWeakObjectPtr<UTextBlock>  TB_AttackIncAmount;
	TWeakObjectPtr<UTextBlock> TB_DefenseIncAmount;
	TWeakObjectPtr<UTextBlock> TB_HealthIncAmount;

	TWeakObjectPtr<UTextBlock> STB_BottomTitle;
	TWeakObjectPtr<UTextBlock>  STB_AttackIncLabel;
	TWeakObjectPtr<UTextBlock> STB_DefenseIncLabel;
	TWeakObjectPtr<UTextBlock> STB_HealthIncLabel;
	TWeakObjectPtr<UTextBlock> STB_EnhanceEfttect;
	
	TArray<TWeakObjectPtr<class UB2RelicUIP_EnchantInfo>> UIP_RelicEnchantInfo;
		
	uint32 bRelicMgmtPartAsyncLoadRequested : 1;

public:

	UB2LobbyUI_RelicMain(const FObjectInitializer& ObjectInitializer);
	
	virtual void CacheAssets() override;
	virtual void DestroySelf() override;
	
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	void OnHeroMgmtPCChanged(EPCClass InSelectedPCClass);

	void UpdateList();
	void CreateList();
	void OnOpenRelicPopup(FAncientRelic relicInfo);

	void SendListRequest(int32 nType = 0, int32 nLevelGrade = 0);

	void DoMarkRedDot_Relic();

	void UpdateStaticText();
private:
	TArray<class UB2RelicUIP_EnchantInfo*> EnchantInfoList;

	uint32 LobbySetHeroMgmtModePCSelectionTicket;
};

UCLASS()
class BLADEII_API UB2LobbyUI_RelicStarItem : public UB2LobbyUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetStarGrade(int32 Grade);
};