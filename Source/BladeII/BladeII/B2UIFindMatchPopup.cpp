// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIFindMatchPopup.h"
#include "BladeII.h"

#include "B2LobbyGameMode.h"
#include "B2UIManager.h"
#include "B2UICharacterSelectPage.h"
#include "B2AndroidBackManager.h"
#include "B2GameDefine.h"


void UB2UIFindMatchPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UButton, BTN_Cancle);

	CharacterSlots.SetNum(CONTROL_GAME_MAX_PLAYERS);
	GET_SLOT_BYNAME(UWidgetSwitcher, CharacterSlots[0], CharacterSlot1);
	GET_SLOT_BYNAME(UWidgetSwitcher, CharacterSlots[1], CharacterSlot2);
	GET_SLOT_BYNAME(UWidgetSwitcher, CharacterSlots[2], CharacterSlot3);
	GET_SLOT_BYNAME(UWidgetSwitcher, CharacterSlots[3], CharacterSlot4);
	GET_SLOT_BYNAME(UWidgetSwitcher, CharacterSlots[4], CharacterSlot5);
	GET_SLOT_BYNAME(UWidgetSwitcher, CharacterSlots[5], CharacterSlot6);

	GET_SLOT(UTextBlock, TB_PlayerCount);
}

void UB2UIFindMatchPopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancle, &UB2UIFindMatchPopup::OnCancleBtnClick);
}

void UB2UIFindMatchPopup::CloseWidgetDelegate()
{
	OnCancleBtnClick();
}

void UB2UIFindMatchPopup::OnCancleBtnClick()
{
	AB2LobbyGameMode* pGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
	{
		if (!pGM->GetRoomId().IsEmpty())
		{
			pGM->LeaveRoomIfJoined();
		}
	}

	OnCancleBtnClickInner();
}

void UB2UIFindMatchPopup::SetPlayerCount(int32 InCount)
{
	if (TB_PlayerCount.IsValid())
	{
		FString PlayerString = TEXT("({0}/{1})");
		TB_PlayerCount->SetText(FText::Format(FText::FromString(PlayerString), FText::AsNumber(InCount), FText::AsNumber(CONTROL_GAME_MAX_PLAYERS)));
	}

	for (int i = 0; i < CharacterSlots.Num(); i++)
	{
		if (CharacterSlots[i].IsValid())
		{
			if (i < InCount)
			{
				CharacterSlots[i]->SetActiveWidgetIndex(1);
			}
			else
			{
				CharacterSlots[i]->SetActiveWidgetIndex(0);
			}
		}
	}
}

void UB2UIFindMatchPopup::OnCancleBtnClickInner()
{
	// 纳腐磐 急琶UI俊 秦临老
	UB2UICharacterSelectPage * pUI = UB2UIManager::GetInstance()->GetUI<UB2UICharacterSelectPage>(UIFName::ControlCharacterSelectPage);

	if (pUI)
	{
		pUI->SetEnableStageGameStarted(false);
	}

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIFindMatchPopup::DestroySelf(UB2UIManager* InUIManager)
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	Super::DestroySelf(InUIManager);
}