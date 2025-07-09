// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "Retailer.h"
#include "B2UIManager.h"
#include "B2UIGuildInfoPart.h"
#include "B2UIPVPEntryHero.h"
#include "B2UIGuildMapCharPopup.h"
#include "B2UIDocHelper.h"
#include "B2UIGuildMapMain.h"
#include "B2UIGuildFormation.h"
#include "B2AndroidBackManager.h"


void UB2UIGuildMapCharPopup::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2RichTextBlock, TB_AllyGuildName);
	GET_SLOT(UB2UIGuildInfoMedal, UIP_AllyGuildMedal1);

	GET_SLOT(UB2RichTextBlock, TB_MedalCount);


	GET_SLOT(UB2UIGuildInfoMedalPart, UIP_GuildAttackInfo1);
	if (UIP_GuildAttackInfo1.IsValid())
	{
		UIP_GuildAttackInfo1->Init();
		UIP_GuildAttackInfo1->SettextAttackNumber(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Map_Info_Attack1")));
	}
	GET_SLOT(UB2UIGuildInfoMedalPart, UIP_GuildAttackInfo2);
	if (UIP_GuildAttackInfo2.IsValid())
	{
		UIP_GuildAttackInfo2->Init();
		UIP_GuildAttackInfo2->SettextAttackNumber(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Map_Info_Attack2")));
	}
	
	GET_SLOT(UB2Button, BTN_Battle);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UOverlay, CP_Battle);


	GET_SLOT(UB2UIGuildFormation, UIP_MyGuildFormation1);

	if (UIP_MyGuildFormation1.IsValid())
	{
		UIP_MyGuildFormation1->Init();
		UIP_MyGuildFormation1->SetTeamNumber(1);
	}

	GET_SLOT(UB2UIGuildFormation, UIP_MyGuildFormation2);

	if (UIP_MyGuildFormation2.IsValid())
	{
		UIP_MyGuildFormation2->Init();
		UIP_MyGuildFormation2->SetTeamNumber(2);
	}

	GET_SLOT(UTextBlock, TB_AllyPowerIndex);

	GET_SLOT(UB2RichTextBlock, TB_EnemyGuildName);
	GET_SLOT(UB2RichTextBlock, TB_EnemyMedalCount);

	GET_SLOT(UB2UIGuildFormation, UIP_EnemyGuildFormation1);
	
	if (UIP_EnemyGuildFormation1.IsValid())
	{
		UIP_EnemyGuildFormation1->Init();
		UIP_EnemyGuildFormation1->SetTeamNumber(1);
	}

	GET_SLOT(UB2UIGuildFormation, UIP_EnemyGuildFormation2);

	if (UIP_EnemyGuildFormation2.IsValid())
	{
		UIP_EnemyGuildFormation2->Init();
		UIP_EnemyGuildFormation2->SetTeamNumber(2);
	}

	GET_SLOT(UPanelWidget, O_AllyPanel);
	GET_SLOT(UPanelWidget, O_EnemyPanel);
	
	GET_SLOT(UTextBlock, TB_EnemyPowerIndex);

}

void UB2UIGuildMapCharPopup::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Battle, &UB2UIGuildMapCharPopup::OnClickBTN_Battle);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIGuildMapCharPopup::OnClickBTN_Close);
}

void UB2UIGuildMapCharPopup::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UIGuildMapCharPopup::Init()
{
	Super::Init();
}

void UB2UIGuildMapCharPopup::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

void UB2UIGuildMapCharPopup::OpenGuildInfo(const b2network::B2GuildBattleMemberPtr& InAllyGuildMember, const TArray<b2network::B2GuildBattleAttackPtr>& AttackInfo)
{
	O_AllyPanel->SetVisibility(ESlateVisibility::Visible);
	O_EnemyPanel->SetVisibility(ESlateVisibility::Hidden);

	//SetGuildInfo(InAllyGuildMember);
	SetAttackInfo(AttackInfo);

	int32 CombatPower1Team = 0;
	int32 CombatPower2Team = 0;
	
	for (auto El : InAllyGuildMember->entry->slots)
	{
		if (El->slot_num > UB2UIGuildFormation::GuildFormationOne)
		{
			UIP_MyGuildFormation2->InitSlotCharacter(El->slot_num - UB2UIGuildFormation::GuildFormationOne, El->entry_code, El->level);
			UIP_MyGuildFormation2->SetSelectType(El->slot_num - UB2UIGuildFormation::GuildFormationOne, EGuildEntrySelectType::NonSelect);
			if (El->entry_code < UB2UIGuildFormation::GuildMercenaryBaseID)
			{
				for (auto CombatEl : InAllyGuildMember->character_powers)
				{
					if (CombatEl->character_type == El->entry_code)
						CombatPower2Team += CombatEl->total_power;
				}
			}

		}
		else
		{
			UIP_MyGuildFormation1->InitSlotCharacter(El->slot_num , El->entry_code, El->level);
			UIP_MyGuildFormation1->SetSelectType(El->slot_num , EGuildEntrySelectType::NonSelect);

			if (El->entry_code < UB2UIGuildFormation::GuildMercenaryBaseID)
			{
				for (auto CombatEl : InAllyGuildMember->character_powers)
				{
					if (CombatEl->character_type == El->entry_code)
						CombatPower1Team += CombatEl->total_power;
				}
			}
		}
		
	}

	UIP_MyGuildFormation1->SetCombat(CombatPower1Team);
	UIP_MyGuildFormation2->SetCombat(CombatPower2Team);


	if (TB_AllyGuildName.IsValid())
		TB_AllyGuildName->SetText(FText::FromString(InAllyGuildMember->nickname));

	if (TB_AllyPowerIndex.IsValid())
		TB_AllyPowerIndex->SetText(FText::AsNumber(InAllyGuildMember->power_index));


	if (UIP_AllyGuildMedal1.IsValid() && TB_MedalCount.IsValid())
	{
		InAllyGuildMember->left_medal ? UIP_AllyGuildMedal1->OnMedal() : UIP_AllyGuildMedal1->OffMedal();
		TB_MedalCount->SetText(FText::FromString(FString::Printf(TEXT("x%d"), InAllyGuildMember->left_medal)));

		BTN_Battle->SetIsEnabled(InAllyGuildMember->left_medal ? true : false);

	}
	
	SetMyGuild();
}

void UB2UIGuildMapCharPopup::OpenGuildInfo(const b2network::B2GuildBattleMemberPtr& InAllyGuildMember, const b2network::B2GuildBattleMemberPtr& InEnemyGuildMember)
{
	//SetGuildInfo(InAllyGuildMember);


	UB2UIDocGuild* GuildDoc = UB2UIDocHelper::GetDocGuild();

	check(GuildDoc)

	if (InAllyGuildMember == nullptr)
		return;

	GuildDoc->SetAllyGuildMemeberEntry(InAllyGuildMember->entry);
	GuildDoc->SetOtherGuildMemeberEntry(InEnemyGuildMember->entry);


	AccountID = InEnemyGuildMember->account_id;

	int32 AllyCombatPower1Team = 0;
	int32 AllyCombatPower2Team = 0;
	int32 EnemyCombatPower1Team = 0;
	int32 EnemyCombatPower2Team = 0;


	O_AllyPanel->SetVisibility(ESlateVisibility::Hidden);
	O_EnemyPanel->SetVisibility(ESlateVisibility::Visible);



	for (auto El : InAllyGuildMember->entry->slots)
	{
		if (El->slot_num > UB2UIGuildFormation::GuildFormationOne)
		{
			UIP_MyGuildFormation2->InitSlotCharacter(El->slot_num - UB2UIGuildFormation::GuildFormationOne, El->entry_code, El->level);
			UIP_MyGuildFormation2->SetSelectType(El->slot_num - UB2UIGuildFormation::GuildFormationOne, EGuildEntrySelectType::NonSelect);
			if (El->entry_code < UB2UIGuildFormation::GuildMercenaryBaseID)
			{
				for (auto CombatEl : InAllyGuildMember->character_powers)
				{
					if (CombatEl->character_type == El->entry_code)
						AllyCombatPower2Team += CombatEl->total_power;
				}
			}
		}
		else
		{
			UIP_MyGuildFormation1->InitSlotCharacter(El->slot_num, El->entry_code, El->level);
			UIP_MyGuildFormation1->SetSelectType(El->slot_num, EGuildEntrySelectType::NonSelect);
			if (El->entry_code < UB2UIGuildFormation::GuildMercenaryBaseID)
			{
				for (auto CombatEl : InAllyGuildMember->character_powers)
				{
					if (CombatEl->character_type == El->entry_code)
						AllyCombatPower1Team += CombatEl->total_power;
				}
			}
		}

	}

	UIP_MyGuildFormation1->SetCombat(AllyCombatPower1Team);
	UIP_MyGuildFormation2->SetCombat(AllyCombatPower2Team);

	if (TB_AllyGuildName.IsValid())
		TB_AllyGuildName->SetText(FText::FromString(InAllyGuildMember->nickname));

	if (TB_AllyPowerIndex.IsValid())
		TB_AllyPowerIndex->SetText(FText::AsNumber(InAllyGuildMember->power_index));


	if (UIP_AllyGuildMedal1.IsValid() && TB_MedalCount.IsValid())
	{
		InAllyGuildMember->left_medal ? UIP_AllyGuildMedal1->OnMedal() : UIP_AllyGuildMedal1->OffMedal();
		TB_MedalCount->SetText(FText::FromString(FString::Printf(TEXT("x%d"), InAllyGuildMember->left_medal)));


	}

	for (auto El : InEnemyGuildMember->entry->slots)
	{
		if (El->slot_num > UB2UIGuildFormation::GuildFormationOne)
		{
			UIP_EnemyGuildFormation2->InitSlotCharacter(El->slot_num - UB2UIGuildFormation::GuildFormationOne, El->entry_code, El->level);
			UIP_EnemyGuildFormation2->SetSelectType(El->slot_num - UB2UIGuildFormation::GuildFormationOne, EGuildEntrySelectType::NonSelect);
			if (El->entry_code < UB2UIGuildFormation::GuildMercenaryBaseID)
			{
				for (auto CombatEl : InEnemyGuildMember->character_powers)
				{
					if (CombatEl->character_type == El->entry_code)
						EnemyCombatPower2Team += CombatEl->total_power;
				}
			}
		}
		else
		{
			UIP_EnemyGuildFormation1->InitSlotCharacter(El->slot_num, El->entry_code, El->level);
			UIP_EnemyGuildFormation1->SetSelectType(El->slot_num, EGuildEntrySelectType::NonSelect);
			if (El->entry_code < UB2UIGuildFormation::GuildMercenaryBaseID)
			{
				for (auto CombatEl : InEnemyGuildMember->character_powers)
				{
					if (CombatEl->character_type == El->entry_code)
						EnemyCombatPower1Team += CombatEl->total_power;
				}
			}

		}

	}

	UIP_EnemyGuildFormation1->SetCombat(EnemyCombatPower1Team);
	UIP_EnemyGuildFormation2->SetCombat(EnemyCombatPower2Team);

	if (TB_EnemyPowerIndex.IsValid())
		TB_EnemyPowerIndex->SetText(FText::AsNumber(InEnemyGuildMember->power_index));

	if (TB_EnemyGuildName.IsValid())
		TB_EnemyGuildName->SetText(FText::FromString(InEnemyGuildMember->nickname));

	if (TB_EnemyMedalCount.IsValid())
		TB_EnemyMedalCount->SetText(FText::FromString(FString::Printf(TEXT("x%d"), InEnemyGuildMember->left_medal)));

	if (BTN_Battle.IsValid())
	{
		BTN_Battle->SetIsEnabled(InEnemyGuildMember->left_medal && InAllyGuildMember->left_attack > 0 ? true : false);
	}

	if (InEnemyGuildMember->is_attacked)
	{
		CP_Battle->SetVisibility(ESlateVisibility::Visible);
		BTN_Battle->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CP_Battle->SetVisibility(ESlateVisibility::Hidden);
		BTN_Battle->SetVisibility(ESlateVisibility::Visible);
	}

	if (UB2UIGuildMapMain::MyGuildInfoState)
		SetMyGuild();
}

void UB2UIGuildMapCharPopup::SetMyGuild()
{
	if (BTN_Battle.IsValid())
		BTN_Battle->SetVisibility(ESlateVisibility::Hidden);
	if(BTN_Battle.IsValid())
		CP_Battle->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIGuildMapCharPopup::SetGuildInfo(const b2network::B2GuildBattleMemberPtr& GuildMember)
{
	if (GuildMember->is_attacked)
	{
		CP_Battle->SetVisibility(ESlateVisibility::Visible);
		BTN_Battle->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CP_Battle->SetVisibility(ESlateVisibility::Hidden);
		BTN_Battle->SetVisibility(ESlateVisibility::Visible);
	}

	if (TB_AllyGuildName.IsValid())
		TB_AllyGuildName->SetText(FText::FromString(GuildMember->nickname));



	if (GuildMember->entry != nullptr)
	{
		for (auto El : GuildMember->entry->slots)
		{
			if (El->slot_num > UB2UIGuildFormation::GuildFormationOne)
			{
				UIP_MyGuildFormation2->InitSlotCharacter(El->slot_num - UB2UIGuildFormation::GuildFormationOne, El->entry_code, El->level);
				UIP_MyGuildFormation2->SetSelectType(El->slot_num - UB2UIGuildFormation::GuildFormationOne, EGuildEntrySelectType::NonSelect);
			}
			else
			{
				UIP_MyGuildFormation1->InitSlotCharacter(El->slot_num, El->entry_code, El->level);
				UIP_MyGuildFormation1->SetSelectType(El->slot_num, EGuildEntrySelectType::NonSelect);
			}
		}
		if (GuildMember->entry->slots.Num() == GetMaxPCClassNum())
		{
			// jwyim GuildEntry
			//UIP_PVPEntry1->SetEntryInfo(IntToPCClass(GuildMember->entry->character_slots[0]->character_type - 1), EEntryType::EntryType_Team, GuildMember->entry->character_slots[0]->character_level);
			//UIP_PVPEntry2->SetEntryInfo(IntToPCClass(GuildMember->entry->character_slots[1]->character_type - 1), EEntryType::EntryType_Team, GuildMember->entry->character_slots[1]->character_level);
			//UIP_PVPEntry3->SetEntryInfo(IntToPCClass(GuildMember->entry->character_slots[2]->character_type - 1), EEntryType::EntryType_Team, GuildMember->entry->character_slots[2]->character_level);
			//UIP_PVPEntry4->SetEntryInfo(IntToPCClass(GuildMember->entry->character_slots[3]->character_type - 1), EEntryType::EntryType_Team, GuildMember->entry->character_slots[3]->character_level);
		}
	}
	

	if (TB_AllyPowerIndex.IsValid())
		TB_AllyPowerIndex->SetText(FText::AsNumber(GuildMember->power_index));
		

	if (UIP_AllyGuildMedal1.IsValid() && TB_MedalCount.IsValid())
	{
		GuildMember->left_medal ? UIP_AllyGuildMedal1->OnMedal() : UIP_AllyGuildMedal1->OffMedal();
		TB_MedalCount->SetText(FText::FromString(FString::Printf(TEXT("x%d"), GuildMember->left_medal)));

		BTN_Battle->SetIsEnabled(GuildMember->left_medal ? true : false);

	}

}

void UB2UIGuildMapCharPopup::SetAttackInfo(const TArray<b2network::B2GuildBattleAttackPtr>& AttackInfo)
{
	int32 AttackNumber = AttackInfo.Num();

	if (AttackNumber >= 1)
	{
		UIP_GuildAttackInfo1->SetVisibility(ESlateVisibility::Visible);
		UIP_GuildAttackInfo1->SetGuildName(false,AttackInfo[0]->target_power_index, AttackInfo[0]->target_nickname);
		UIP_GuildAttackInfo1->SetMedalCount(AttackInfo[0]->obtain_medal);
		UIP_GuildAttackInfo1->SetIsAttack(true);

	}
	else
		UIP_GuildAttackInfo1->SetIsAttack(false);

	if (AttackNumber == 2)
	{
		UIP_GuildAttackInfo2->SetVisibility(ESlateVisibility::Visible);
		UIP_GuildAttackInfo2->SetGuildName(false,AttackInfo[1]->target_power_index, AttackInfo[1]->target_nickname);
		UIP_GuildAttackInfo2->SetMedalCount(AttackInfo[1]->obtain_medal);
		UIP_GuildAttackInfo2->SetIsAttack(true);
	}
	else
		UIP_GuildAttackInfo2->SetIsAttack(false);
}

void UB2UIGuildMapCharPopup::SetCloseDelegate(FGuildPopupClose Delegate)
{
	GuildPopupClose = Delegate;
}

void UB2UIGuildMapCharPopup::SetVisibilityPopup(bool InVisible)
{
	if (InVisible)
	{
		SetVisibility(ESlateVisibility::Visible);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	} 
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	}
}

void UB2UIGuildMapCharPopup::OnClickBTN_Battle()
{
	UB2UIDocGuild* GuildDoc = UB2UIDocHelper::GetDocGuild();

	check(GuildDoc)

	GuildDoc->SetOtherGuildMemeberAccountId(AccountID);
	data_trader::Retailer::GetInstance().RequestStartGuildBattle(AccountID);
}

void UB2UIGuildMapCharPopup::OnClickBTN_Close()
{
	GuildPopupClose.ExecuteIfBound();
	SetVisibilityPopup(false);
}


