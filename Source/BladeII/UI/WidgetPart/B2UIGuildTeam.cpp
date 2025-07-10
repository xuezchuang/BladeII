// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIGuildTeam.h"
#include "BladeIILocText.h"


UB2UIGuildTeam::UB2UIGuildTeam(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2UIGuildTeam::Init()
{
	Super::Init();
}
void UB2UIGuildTeam::SetData(int32 InTeamIndex)
{
	const FString Key = FString::Printf(TEXT("Guild_Team_0%d"), InTeamIndex + 1);
	FText TeamNumText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, Key);
	for (TWeakObjectPtr<UTextBlock>& TextBlock : X_TB_TeamNums)
	{
		if (TextBlock.IsValid())
			TextBlock->SetText(TeamNumText);
	}
}

void UB2UIGuildTeam::CacheAssets()
{
	GET_SLOT(UOverlay, X_O_WaitBattle);
	GET_SLOT(UOverlay, X_O_StartBattle);
	GET_SLOT(UOverlay, X_O_EndBattle);

	X_TB_TeamNums.Empty();
	for (uint8 i = (uint8)EGuildTeamState::Waiting; i < (uint8)EGuildTeamState::Max; i++)
		X_TB_TeamNums.Add(GetCachedWidget<UTextBlock>(FName(*FString::Printf(TEXT("X_TB_TeamNum%u"), i))));
}

void UB2UIGuildTeam::SetBattleState(EGuildTeamState GuildTeamState)
{
	if (X_O_WaitBattle.IsValid()) 
		X_O_WaitBattle->SetVisibility(GuildTeamState == EGuildTeamState::Waiting ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (X_O_StartBattle.IsValid())
		X_O_StartBattle->SetVisibility(GuildTeamState == EGuildTeamState::Start ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (X_O_EndBattle.IsValid())
		X_O_EndBattle->SetVisibility(GuildTeamState == EGuildTeamState::End ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
