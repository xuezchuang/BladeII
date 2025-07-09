// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIUserWidget.h"
#include "B2LobbyUserWidget.generated.h"

/**
 * To contain some common stuff for Lobby UI.
 */
UCLASS()
class BLADEII_API UB2LobbyUserWidget : public UBladeIIUserWidget
{
	GENERATED_BODY()
	
protected:

	/** Some sort of middle way between game code and ui code. Could be temporary structure. */
	UPROPERTY(Transient)
	class UB2UIManager_Lobby* OwnerUIManager;

	UPROPERTY(Transient)
	class AB2LobbyGameMode* CachedLobbyGM;

	// Anything else..
	
public:

	UB2LobbyUserWidget(const FObjectInitializer& ObjectInitializer);
	
	/** Supposed to be called right after the creation. Ensured by LobbyUISwitcher. */
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM);

protected:
	virtual void CacheAssets() override;

protected:
	TWeakObjectPtr<class UB2UIInvenQuickSlide> UIP_InvenQuickSlide;
};
