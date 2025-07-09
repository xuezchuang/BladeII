// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIGuildInfoPart.h"
#include "B2UIGuildBattleSiuation.h"
#include "B2UIGuildMapMain.h"
#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIGuildBattleSiuation::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2UIGuildInfoPart4, UIP_LeftBattleGuildInfo);
	if (UIP_LeftBattleGuildInfo.IsValid()) UIP_LeftBattleGuildInfo->Init();
	GET_SLOT(UB2UIGuildInfoPart4, UIP_RightBattleGuildInfo);
	if (UIP_RightBattleGuildInfo.IsValid()) UIP_RightBattleGuildInfo->Init();
	GET_SLOT(UB2RichTextBlock, TB_Time);
	GET_SLOT(UB2Button, BTN_Division);
	GET_SLOT(UB2Button, BTN_Division1);
	GET_SLOT(UB2Button, BTN_Division2);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UVerticalBox, VB_List);
	GET_SLOT(UCanvasPanel, CP_Tap);

	GET_SLOT(UB2RichTextBlock , TB_Division)
	GET_SLOT(UB2RichTextBlock, TB_Division1)
	GET_SLOT(UB2RichTextBlock, TB_Division2)

	GuildBattleType = EUIGuildBattleSiuation::Total;
}

void UB2UIGuildBattleSiuation::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Division, &UB2UIGuildBattleSiuation::OnClickBTN_Division);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close , &UB2UIGuildBattleSiuation::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_Division1, &UB2UIGuildBattleSiuation::OnClickBTN_Division1);
	BIND_CLICK_FUNC_TO_BTN(BTN_Division2, &UB2UIGuildBattleSiuation::OnClickBTN_Division2);
}

void UB2UIGuildBattleSiuation::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UIGuildBattleSiuation::Init()
{
	Super::Init();
}

void UB2UIGuildBattleSiuation::DestroySelf(UB2UIManager* InUIManager)
{

	if(UIP_LeftBattleGuildInfo.IsValid()) UIP_LeftBattleGuildInfo->DestroySelf(InUIManager);
	if(UIP_RightBattleGuildInfo.IsValid()) UIP_RightBattleGuildInfo->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}


void UB2UIGuildBattleSiuation::OnOpen(bool RightNow /*= false*/)
{
	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	CP_Tap->SetVisibility(ESlateVisibility::Hidden);
	SetGuildInfo();
	TotalGuildMatchList();
}


void UB2UIGuildBattleSiuation::SetGuildInfo()
{
	auto& ClientData = BladeIIGameImpl::GetClientDataStore();

	auto MyGuildInfo = ClientData.GetMyGuildBattleInfo();
	auto OtherGuildInfo = ClientData.GetOtherGuildBattleInfo();

	if (UIP_LeftBattleGuildInfo.IsValid())
	{
		UIP_LeftBattleGuildInfo->SetGuildInfo(FText::FromString(MyGuildInfo->guild_name), MyGuildInfo->attack_count,
			MyGuildInfo->members.Num() * UB2UIGuildMapMain::MaxAttackCount, MyGuildInfo->obtain_medal, MyGuildInfo->members.Num() * UB2UIGuildMapMain::MaxMedal);
		UIP_LeftBattleGuildInfo->SetGuildMark(MyGuildInfo->mark_index, MyGuildInfo->mark_color, MyGuildInfo->mark_bg_index, MyGuildInfo->mark_bg_color);
	}

	if (UIP_RightBattleGuildInfo.IsValid())
	{
		UIP_RightBattleGuildInfo->SetGuildInfo(FText::FromString(OtherGuildInfo->guild_name), OtherGuildInfo->attack_count,
			OtherGuildInfo->members.Num() * UB2UIGuildMapMain::MaxAttackCount, OtherGuildInfo->obtain_medal, OtherGuildInfo->members.Num() * UB2UIGuildMapMain::MaxMedal);
		UIP_RightBattleGuildInfo->SetGuildMark(OtherGuildInfo->mark_index, OtherGuildInfo->mark_color, OtherGuildInfo->mark_bg_index, OtherGuildInfo->mark_bg_color);
	}
}

void UB2UIGuildBattleSiuation::MyGuildMatchList()
{
	VB_List->ClearChildren();

	auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	for (auto El : ClientData.GetMyGuildBattleInfo()->attacks)
	{
		auto* ListSlot = CreateWidget<UB2UIGuildBattleSituationSlot>(GetWorld(), SituationSlot);
		if (!ListSlot)
			return;

		VB_List->AddChild(ListSlot);
		ListSlot->Init();
		ListSlot->SetSituationSlot(El->power_index, El->nickname, El->target_power_index, El->target_nickname, El->obtain_medal, true, El->battle_end_time);
	}
}


void UB2UIGuildBattleSiuation::OtherGuildMatchList()
{
	VB_List->ClearChildren();

	auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	for (auto El : ClientData.GetOtherGuildBattleInfo()->attacks)
	{
		auto* ListSlot = CreateWidget<UB2UIGuildBattleSituationSlot>(GetWorld(), SituationSlot);
		if (!ListSlot)
			return;

		VB_List->AddChild(ListSlot);
		ListSlot->Init();
		ListSlot->SetSituationSlot( El->target_power_index, El->target_nickname, El->power_index,  El->nickname, El->obtain_medal, false, El->battle_end_time);
	}
}


void UB2UIGuildBattleSiuation::TotalGuildMatchList()
{
	VB_List->ClearChildren();

	auto& ClientData = BladeIIGameImpl::GetClientDataStore();

	b2network::B2GuildBattleGuildPtr MyGuild = ClientData.GetMyGuildBattleInfo();
	b2network::B2GuildBattleGuildPtr OtherGuild = ClientData.GetOtherGuildBattleInfo();

	TArray<b2network::B2GuildBattleAttackPtr> Total = MyGuild->attacks;
	Total.Append(OtherGuild->attacks);

	Total.Sort([](b2network::B2GuildBattleAttackPtr Ele1, b2network::B2GuildBattleAttackPtr Ele2)->bool{
		if (Ele1->battle_end_time < Ele2->battle_end_time) return false; else return true;
	});

	for (auto El : Total)
	{
		auto* ListSlot = CreateWidget<UB2UIGuildBattleSituationSlot>(GetWorld(), SituationSlot);
		if (!ListSlot)
			return;

		VB_List->AddChild(ListSlot);
		ListSlot->Init();
		ListSlot->SetGuildMark(true, MyGuild->mark_index, MyGuild->mark_color, MyGuild->mark_bg_index, MyGuild->mark_bg_color);
		ListSlot->SetGuildMark(false, OtherGuild->mark_index, OtherGuild->mark_color, OtherGuild->mark_bg_index, OtherGuild->mark_bg_color);

		if (El->guild_id == ClientData.GetMyGuildBattleInfo()->guild_id)
		{
			ListSlot->SetSituationSlot(El->power_index, El->nickname, El->target_power_index, El->target_nickname, El->obtain_medal,
				true , El->battle_end_time);
		}
		else
		{
			ListSlot->SetSituationSlot(El->target_power_index, El->target_nickname, El->power_index, El->nickname, El->obtain_medal,
				false, El->battle_end_time);
		}
	}
}


void UB2UIGuildBattleSiuation::SetDivisionText()
{
	if (GuildBattleType == EUIGuildBattleSiuation::Total)
	{
		TB_Division->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_TotalAttack")));
		TB_Division1->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_MyAttack")));
		TB_Division2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_OtherAttack")));
	}
	else if (GuildBattleType == EUIGuildBattleSiuation::MyGuild)
	{
		TB_Division->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_MyAttack")));
		TB_Division1->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_TotalAttack")));
		TB_Division2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_OtherAttack")));
	}
	else
	{
		TB_Division->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_OtherAttack")));
		TB_Division1->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_TotalAttack")));
		TB_Division2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_MyAttack")));
	}

	CP_Tap->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIGuildBattleSiuation::OnClickBTN_Division()
{
	if (CP_Tap.IsValid())
	{
		if (CP_Tap->GetVisibility() == ESlateVisibility::Visible)
			CP_Tap->SetVisibility(ESlateVisibility::Hidden);
		else
			CP_Tap->SetVisibility(ESlateVisibility::Visible);
	}
}


void UB2UIGuildBattleSiuation::OnClickBTN_Division1()
{
	if (GuildBattleType == EUIGuildBattleSiuation::Total)
	{
		MyGuildMatchList();
		GuildBattleType = EUIGuildBattleSiuation::MyGuild;
	}
	else
	{
		TotalGuildMatchList();
		GuildBattleType = EUIGuildBattleSiuation::Total;
	}

	SetDivisionText();
}


void UB2UIGuildBattleSiuation::OnClickBTN_Division2()
{
	
	if (GuildBattleType == EUIGuildBattleSiuation::TargetGuild)
	{
		MyGuildMatchList();
		GuildBattleType = EUIGuildBattleSiuation::MyGuild;
	}
	else
	{
		OtherGuildMatchList();
		GuildBattleType = EUIGuildBattleSiuation::TargetGuild;
	}
	
	SetDivisionText();
}

void UB2UIGuildBattleSiuation::OnClickClose()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	SetVisibility(ESlateVisibility::Hidden);
}


