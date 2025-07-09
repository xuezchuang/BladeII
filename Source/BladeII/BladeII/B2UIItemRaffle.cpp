// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIItemRaffle.h"
#include "B2LobbyUI_ItemDetailPostOp.h"
#include "B2UIManager.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2AndroidBackManager.h"

#include "BladeIIGameImpl.h"

void UB2UIItemRaffle::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2LobbyUI_ItemDetailPostOp, ResultPage);
	if (ResultPage.IsValid())
		ResultPage->CacheAssets();
}

void UB2UIItemRaffle::BindDelegates()
{
	Super::BindDelegates();

	if (ResultPage.IsValid())
		BIND_CLICK_FUNC_TO_BTN(ResultPage->BTN_Receive, &UB2UIItemRaffle::OnClickButton);

	if (ResultPage.IsValid())
		BIND_CLICK_FUNC_TO_BTN(ResultPage->BTN_Confirm, &UB2UIItemRaffle::OnClickButton);
}

void UB2UIItemRaffle::CloseWidgetDelegate()
{
	OnClickButton();
}

void UB2UIItemRaffle::Init()
{
	Super::Init();

	CacheAssets();
}

void UB2UIItemRaffle::DestroySelf(UB2UIManager* InUIManager)
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	Super::DestroySelf(InUIManager);
}

void UB2UIItemRaffle::StartFromRaffle(const FB2Item& ItemInfo)
{
	if (ResultPage.IsValid())
	{
		ResultPage->StartFromRaffle(ItemInfo);
	}

	B2AndroidBackManager::GetInstance()->AddBackWidget(this);

	TArray<FB2Item> RaffleItem;
	RaffleItem.Add(ItemInfo);
	BladeIIGameImpl::GetClientDataStore().AddNewAcquiredItems(RaffleItem);
}

void UB2UIItemRaffle::OnClickButton()
{
	DestroySelf(UB2UIManager::GetInstance());

	if (AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (UB2LobbyInventory* LobbyInven = LobbyGM->GetLobbyInventory())
		{
			LobbyInven->ConditionalCheckForBetterEquipSuggest();
		}
	}

	if (ResultPage.IsValid())
		ResultPage->DestroySelf();
}
