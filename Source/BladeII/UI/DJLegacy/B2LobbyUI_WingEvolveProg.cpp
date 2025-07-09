// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUI_WingEvolveProg.h"
#include "B2LobbyGameMode.h"
#include "B2Airport.h"
#include "Event.h"

UB2LobbyUI_WingEvolveProg::UB2LobbyUI_WingEvolveProg(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{


}

void UB2LobbyUI_WingEvolveProg::CacheAssets()
{
	Super::CacheAssets();

	
}

void UB2LobbyUI_WingEvolveProg::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);


	UpdateStaticWidgets();
}

void UB2LobbyUI_WingEvolveProg::DestroySelf()
{
	Super::DestroySelf();

}

void UB2LobbyUI_WingEvolveProg::UpdateStaticWidgets()
{
	
}
