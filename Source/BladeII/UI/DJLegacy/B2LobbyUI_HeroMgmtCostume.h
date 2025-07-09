// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DJLegacy/B2LobbyUI_HeroMgmtCommon.h"
#include "B2LobbyUI_HeroMgmtCostume.generated.h"

/**
 * 
 */

class issue;
using issue_ptr = std::shared_ptr<issue>;
UCLASS()
class BLADEII_API UB2LobbyUI_HeroMgmtCostume : public UB2LobbyUI_HeroMgmtCommon
{
	GENERATED_BODY()

public:
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	virtual void DestroySelf() override;

	virtual void OnHeroPCChanged(EPCClass UpdatedClass) override;

protected:
	virtual void InternalSubscribeEvents() override;
	virtual void InternalUnsubscribeEvents() override;

	virtual bool RedDotCondition_ByCostumeMode(EPCClass PCClass) override;

private:
	void HeroMgmtCostume_Init();
	void HeroMgmtCostume_CacheAssets();
	void HeroMgmtCostume_BindDelegates();

	void SyncCostumeButton();

protected:
	UFUNCTION()
	void OnShowCostume_HeadButtonPressed(bool IsCheck);

	UFUNCTION()
	void OnShowCostume_WeaponButtonPressed(bool IsCheck);

	UFUNCTION()
	void OnShowCostume_AmorButtonPressed(bool IsCheck);

	UFUNCTION()
	void OnClickBTN_HeroInfo();

	UFUNCTION()
	void OnChangeVisible(EPCClass InPCClass, ECostumeEquipPlace InEquipPlace, bool bVisible);

	UFUNCTION()
	void OnChangeVisibleMeshPart(EPCClass InPCClass, ECostumeEquipPlace InEquipPlace, bool bVisible);

	UFUNCTION()
	void OnChangeVisibleMode(bool bPreview);

private:
	TWeakObjectPtr<UCheckBox> CB_ShowCostume_Head;
	TWeakObjectPtr<UCheckBox> CB_ShowCostume_Weapon;
	TWeakObjectPtr<UCheckBox> CB_ShowCostume_Amor;

	TWeakObjectPtr<UB2Button> BTN_HeroInfo;

	TArray<issue_ptr> Issues;
};
