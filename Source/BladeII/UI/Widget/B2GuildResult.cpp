// Fill out your copyright notice in the Description page of Project Settings.


#include "B2GuildResult.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2GuildGameMode.h"
#include "B2UIGuildInfoPart.h"
#include "B2GuildGameMode.h"
#include "Retailer.h"
#include "B2UIRewardEventSet.h"

void UB2GuildResult::Init()
{
	Super::Init();
}

void UB2GuildResult::OnOpenComplete()
{
	Super::OnOpenComplete();

	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	if (!GuildDoc)
		return;

	int32  i = 0;
	for (auto& El : MedalArr)
	{
		if (GuildDoc->GetBattleResultMedal() > i)
			El->OnMedal();
		else
			El->OffMedal();

		++i;
	}

	if (!X_RewardBox.IsValid())
		return;

	X_RewardBox->ClearChildren();

	TArray<b2network::B2RewardPtr> GuildRewardItem = GuildDoc->GetBattleResultReward();

	for (auto Rewarditem : GuildRewardItem)
	{
		if (Rewarditem->raw_reward->id == 0)
			return;

		auto* RewardWidget = CreateWidget<UB2UIRewardIcon>(GetWorld(), GuildRewardSlot);

		if (!RewardWidget)
			return;

		UHorizontalBoxSlot* BoxSlot =  Cast<UHorizontalBoxSlot>(X_RewardBox->AddChild(RewardWidget));
		RewardWidget->Init();
		RewardWidget->UpdateRewardIcon(Rewarditem);

		if (!BoxSlot)
			return;

		BoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		BoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		BoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	}

	if (TB_Error.IsValid())
	{
		TB_Error->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Guild_BattleResponseError"))));
		TB_Error->SetVisibility(ESlateVisibility::Collapsed);
	}

	auto* DocSome = UB2UIDocHelper::GetDocSome();
	bool bWin = DocSome && (DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin);

	OnOpenComplete_BP(GuildDoc->GetBattleResultMedal(), bWin);
}

void UB2GuildResult::BindDocAuto()
{
	auto* DocSome = UB2UIDocHelper::GetDocSome();
	if (DocSome && (DocSome->GetLocalPlayerMatchResult() == ENetMatchResult::LocalWin))
	{
		IMG_Victory->SetVisibility(ESlateVisibility::Visible);
		IMG_Lose->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		IMG_Lose->SetVisibility(ESlateVisibility::Visible);
		IMG_Victory->SetVisibility(ESlateVisibility::Hidden);
	}
	// 眠饶 鞘夸俊 蝶扼 draw 贸府.
}

void UB2GuildResult::UnbindDoc()
{

}

void UB2GuildResult::UpdateStaticText()
{
	if (TB_BtnMain.IsValid())
		TB_BtnMain->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("MainScreen"))));
	if (TB_BtnGuild.IsValid())
		TB_BtnGuild->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("ChallengeMod_GuildMatch"))));
}

void UB2GuildResult::CacheAssets()
{
	GET_SLOT(UButton, BTN_GUILD);
	GET_SLOT(UButton, BTN_MAIN);
	GET_SLOT(UImage, IMG_Lose);
	GET_SLOT(UImage, IMG_Victory);
	GET_SLOT(UHorizontalBox, X_RewardBox);
	GET_SLOT(UTextBlock, TB_Error);
	GET_SLOT(UTextBlock, TB_BtnMain);
	GET_SLOT(UTextBlock, TB_BtnGuild);

	MedalArr.SetNum(4);
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, MedalArr[0], UIP_GuildMedal1);
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, MedalArr[1], UIP_GuildMedal2);
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, MedalArr[2], UIP_GuildMedal3);
	GET_SLOT_BYNAME(UB2UIGuildInfoMedal, MedalArr[3], UIP_GuildMedal4);
}

void UB2GuildResult::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_GUILD, &UB2GuildResult::OnClickBtnGuild);
	BIND_CLICK_FUNC_TO_BTN(BTN_MAIN, &UB2GuildResult::OnClickBtnMain);
}

void UB2GuildResult::OnClickBtnGuild()
{
	auto* GameMode = Cast<AB2GuildGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->ReturnToGuildMap();
	}

}

void UB2GuildResult::OnClickBtnMain()
{
	auto* GameMode = Cast<AB2GuildGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->GoToVillage();
	}

}

