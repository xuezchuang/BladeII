// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/DJLegacy/B2LobbyUserWidget.h"
#include "CommonStruct.h"
#include "B2LobbyUI_WingEvolveProg.generated.h"

/**
 * UI page for Wing evolution event scene
 */
UCLASS()
class BLADEII_API UB2LobbyUI_WingEvolveProg : public UB2LobbyUserWidget
{
	GENERATED_BODY()
	
protected:
	
	//////////////////////////////////////////////////
	//
	// It probably got nothing..
	//
	//////////////////////////////////////////////////


public:

	UB2LobbyUI_WingEvolveProg(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	
	void DestroySelf() override;

private:
	
	void UpdateStaticWidgets();
		

protected:

	

};
