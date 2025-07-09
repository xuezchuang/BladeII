// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUserWidget.h"
#include "B2UIManager_Lobby.h"
#include "B2UIInvenQuickSlide.h"

UB2LobbyUserWidget::UB2LobbyUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OwnerUIManager = NULL;
	CachedLobbyGM = NULL;
}

void UB2LobbyUserWidget::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	OwnerUIManager = InUIManager;
	CachedLobbyGM = InGM;
	check(OwnerUIManager && CachedLobbyGM);
}

void UB2LobbyUserWidget::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2UIInvenQuickSlide, UIP_InvenQuickSlide);
	if (UIP_InvenQuickSlide.IsValid())
		UIP_InvenQuickSlide->Init();
}